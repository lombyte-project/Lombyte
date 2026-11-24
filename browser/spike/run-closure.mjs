// Runs a module from link-closure.sh and reports how far the original code gets. Every missing
// function is an import that throws with its symbol name, so the first trap names the next
// service to supply. Nothing here claims the run is a correct game frame, only which original
// code executed and where it stopped.
//
// Usage: node browser/spike/run-closure.mjs <name> <root symbol> [--calls N] [--args a,b,...]
//          [--elf path/to/SCUS_971.99] [--level N] [--poke32 0xADDRESS=VALUE]... [--scenario file.mjs]
//
// --elf copies the boot ELF's segments to their addresses; --level N does the same for level N's
// program data (config/us/overlays/level_NN, from scripts/overlay-extract.py): .lit, .data and the
// three vtbl records. Code records are not loaded: code runs from the compiled C.
//
// Without --args the root is called as root(moby): a zeroed moby whose pvars pointer (+0x78)
// points at a zeroed block, both in unused high RAM.
// A scenario module may export:
//   services(context)        -> { importName: function }   real services for this run
//   async setup(context)     -> optional { args } or       stage real state before the calls
//                               { sequence: [{ symbol, args }] }  to call several roots in order;
//                               a { host, label } step runs a scenario function between calls
//   deferred                 -> { importName: reason }     output-only work with no consumer yet;
//                                                           returns 0, is counted, and is listed
//                                                           at the end of every run as NOT DONE
//   report(context, results)                               print what the run produced
// context = { memory, view(), bytes(), exports, cString }.
import { execFileSync } from 'node:child_process';
import { deflateSync } from 'node:zlib';
import { GsRenderer } from '../gs-renderer.mjs';
import { parseVif, readChain } from './display-list.mjs';
import { existsSync, readFileSync, writeFileSync } from 'node:fs';
import { dirname, join, resolve } from 'node:path';
import { fileURLToPath, pathToFileURL } from 'node:url';

const root = join(dirname(fileURLToPath(import.meta.url)), '..', '..');
const [name, rootSymbol] = process.argv.slice(2);
const option = (flag, fallback) => {
  const index = process.argv.indexOf(flag);
  return index > 0 ? process.argv[index + 1] : fallback;
};
const calls = Number(option('--calls', '3'));
const elfPath = option('--elf', null);
const scenarioPath = option('--scenario', null);
let rootArgs = option('--args', null)?.split(',').map(Number) ?? null;
const scenario = scenarioPath ? await import(pathToFileURL(resolve(scenarioPath)).href) : {};

const module = new WebAssembly.Module(readFileSync(join(root, 'build/spike', `${name}.wasm`)));
const context = {
  memory: null,
  exports: null,
  view() { return new DataView(this.memory.buffer); },
  bytes() { return new Uint8Array(this.memory.buffer); },
  cString(address) {
    const bytes = this.bytes();
    let end = address;
    while (bytes[end]) end += 1;
    return new TextDecoder().decode(bytes.subarray(address, end));
  },
};
const services = scenario.services?.(context) ?? {};
const deferred = { ...(scenario.deferred ?? {}) };
const deferredCalls = new Map();
let trapCount = 0;
let debugLines = 0;
// The level program the boot program loads (parse_bin) runs from its own module: --handover <name>
// names it (build/spike/<name>.wasm, e.g. level). See handOver below.
const handoverName = option('--handover', null);
class Handover extends Error {}
function buildImports(module) {
const imports = {};
for (const { module: from, name: symbol, kind } of WebAssembly.Module.imports(module)) {
  imports[from] ??= {};
  if (kind !== 'function') throw new Error(`unexpected ${kind} import ${symbol}`);
  if (services[symbol]) {
    imports[from][symbol] = services[symbol];
  } else if (deferred[symbol]) {
    imports[from][symbol] = () => { deferredCalls.set(symbol, (deferredCalls.get(symbol) ?? 0) + 1); return 0; };
  } else if (symbol === 'sinf' || symbol === 'cosf') { // host trig behind fast_sin/fast_cos (see ee-helpers.c)
    imports[from][symbol] = (x) => Math.fround(symbol === 'sinf' ? Math.sin(x) : Math.cos(x));
  } else if (symbol === 'spike_deferred') { // unimplemented subsystems named in services.c
    imports[from][symbol] = (nameAt, reasonAt) => {
      const key = context.cString(nameAt);
      deferred[key] ??= context.cString(reasonAt);
      deferredCalls.set(key, (deferredCalls.get(key) ?? 0) + 1);
    };
  } else if (symbol === 'spike_debug_print') { // the game's own printf (debug text)
    imports[from][symbol] = (address) => { const text = context.cString(address).replace(/\n$/, ''); if (debugLines++ < 40) console.log(`game printf: ${text}`); };
  } else if (symbol === 'spike_pad_buttons') { // the controller (services.c scePad2Read): --pad script
    imports[from][symbol] = () => padButtonsAt(hostFrames);
  } else if (symbol === 'spike_play_movie') { // services.c play_mpeg_movie: no player in Node
    imports[from][symbol] = (sector, sectors) => {
      moviesPlayed.push({ sector, sectors, frame: hostFrames });
      console.log(`movie requested at host frame ${hostFrames}: disc sector ${sector}, ${sectors} sectors (not shown in Node)`);
    };
  } else if (symbol === 'spike_gs_load_image') { // services.c sceGsExecLoadImage -> the GS backend
    imports[from][symbol] = (dbp, dbw, psm, x, y, w, h, source) => gsLoadImage(dbp, dbw, psm, x, y, w, h, source);
  } else if (symbol === 'spike_dma_chain') { // services.c sceDmaSend (VIF1, GIF) -> the GS backend
    imports[from][symbol] = (channel, chain) => gsDmaChain(channel >>> 0, chain >>> 0);
  } else if (symbol === 'spike_read_clock') { // services.c sceCdReadClock: the host clock, BCD
    imports[from][symbol] = (at) => { const now = new Date(); const bcd = (n) => ((Math.floor(n / 10) << 4) | (n % 10)); const b = context.bytes(); b.set([0, bcd(now.getSeconds()), bcd(now.getMinutes()), bcd(now.getHours()), 0, bcd(now.getDate()), bcd(now.getMonth() + 1), bcd(now.getFullYear() % 100)], at); };
  } else if (symbol === 'spike_host_frame') {
    imports[from][symbol] = () => hostFrames;
  } else if (symbol === 'spike_vsync_yield') { // the frame boundary (services.c, Asyncify)
    imports[from][symbol] = () => frameYield();
  } else if (symbol === 'spike_unimplemented_rpc') {
    imports[from][symbol] = (server, fn) => { throw new Error(`IOP RPC server 0x${server.toString(16)} is not implemented (function ${fn})`); };
  } else if (symbol === 'spike_conflict') {
    imports[from][symbol] = (address) => { throw new Error(`missing function with conflicting caller prototypes: ${context.cString(address)}`); };
    trapCount += 1;
  } else if (symbol === 'spike_pad_sticks') { // --stick "from-to:lx,ly" (bytes, 0x80 centre); centred otherwise
    imports[from][symbol] = () => {
      for (const { from: a, to: b, lx, ly } of stickScript) if (hostFrames >= a && hostFrames < b) return ((0x80 | 0x80 << 8 | lx << 16 | ly << 24) >>> 0);
      return 0x80808080;
    };
  } else if (symbol === 'spike_world_pass') { // services.c tfrag_proc: the page draws the world here
    imports[from][symbol] = () => {};
  } else if (symbol === 'spike_program_loaded') { // services.c FlushCache(2)
    imports[from][symbol] = () => { const entry = loadedProgramEntry(); if (entry) throw new Handover(`level program loaded, entry 0x${entry.toString(16)}`); };
  } else {
    imports[from][symbol] = () => { throw new Error(`missing function: ${symbol}`); };
    trapCount += 1;
  }
}
return imports;
}
const imports = buildImports(module);
// The program parse_bin has just copied in: level_root (D_0015EE4C) points at the loaded level
// file, whose first word is the offset of its chunk list { dst, size, pad, id } + payload. The
// copy has happened when the first chunk's payload is at its destination; parse_bin returns the
// chunk id, the program's entry address. Only the boot program hands over, once.
let handedOver = false;
function loadedProgramEntry() {
  if (!handoverName || handedOver) return 0;
  const view = context.view(), bytes = context.bytes();
  const rootAt = view.getUint32(0x15EE4C, true);
  if (!rootAt || rootAt >= 0x2000000) return 0;
  const chunk = rootAt + view.getUint32(rootAt, true);
  const dst = view.getUint32(chunk, true), size = view.getInt32(chunk + 4, true), id = view.getUint32(chunk + 12, true);
  if (!dst || dst >= 0x2000000 || size <= 0 || size > 0x2000000) return 0;
  const n = Math.min(size, 4096);
  for (let i = 0; i < n; i++) if (bytes[dst + i] !== bytes[chunk + 16 + i]) return 0;
  const level = levelForEntry(id);
  return level === null ? 0 : id;
}
function levelForEntry(entry) {
  for (let level = 0; level < 20; level++) {
    try {
      const manifest = JSON.parse(readFileSync(join(root, 'config/us/overlays', `level_${String(level).padStart(2, '0')}`, 'manifest.json'), 'utf8'));
      if (manifest.entry_point === entry) return level;
    } catch { /* no such level extracted */ }
  }
  return null;
}
// The console jumps to the loaded program's entry: here the level program's own module takes the
// machine over. EE RAM, the scratchpad and the hardware register pages are copied into it (each
// module's own C statics sit above the RAM image, 0x2000000, and are not copied: services.c state
// starts fresh there), the scenario does what the load means for this build (handler tables,
// scratchpad pointers), and the entry runs.
function handOver(entry) {
  const level = levelForEntry(entry);
  context.previousVblankRuns = context.exports.spike_vblank_runs?.() ?? 0;
  context.previousCdCallback = context.exports.spike_cd_callback_installed?.() ?? 0;
  const from = context.bytes();
  const next = new WebAssembly.Module(readFileSync(join(root, 'build/spike', `${handoverName}.wasm`)));
  const nextInstance = new WebAssembly.Instance(next, buildImports(next));
  const memory = nextInstance.exports.memory;
  if (memory.buffer.byteLength < from.length) memory.grow(Math.ceil((from.length - memory.buffer.byteLength) / 65536));
  const to = new Uint8Array(memory.buffer);
  for (const [start, end] of [[0xF0000, 0xF4000], [0x100000, 0x2000000], [0x10000000, 0x10010000], [0x12000000, 0x12002000]]) to.set(from.subarray(start, end), start);
  instance = nextInstance;
  context.memory = memory;
  context.exports = nextInstance.exports;
  handedOver = true;
  scenario.onHandover?.(context, level);
  return `FUN_L${String(level).padStart(2, '0')}_${entry.toString(16).padStart(8, '0')}`;
}
// Asyncify state for frames steps: a vsync inside one unwinds to the host; the next frame
// rewinds into the same call. Outside a frames step the yield returns at once.
const ASYNCIFY_DATA = 0x90000, ASYNCIFY_END = 0xE0000;
let frameMode = 'off';
let lastYieldStack = ''; // off | running | unwinding | rewinding
// --pad "120:start,180:cross+up": buttons held during host frame N (the frame after the Nth
// vsync yield) and the next PAD_HOLD frames. Names are the DualShock's; the mask uses the
// controller report's bit order (scePad2Read: byte 0 then byte 1, here 1 = pressed).
// include/rnc/input/pad_state.h (PAD_L2 0x0001 ... PAD_LEFT 0x8000).
const PAD_BITS = { l2: 0x0001, r2: 0x0002, l1: 0x0004, r1: 0x0008, triangle: 0x0010, circle: 0x0020,
  cross: 0x0040, square: 0x0080, select: 0x0100, l3: 0x0200, r3: 0x0400, start: 0x0800, up: 0x1000,
  right: 0x2000, down: 0x4000, left: 0x8000 };
const PAD_HOLD = 6;
const padScript = (option('--pad', '') || '').split(',').filter(Boolean).map((entry) => {
  const [frame, names] = entry.split(':');
  const mask = names.split('+').reduce((bits, name) => {
    if (!(name in PAD_BITS)) throw new Error(`--pad: unknown button ${name}`);
    return bits | PAD_BITS[name];
  }, 0);
  return { frame: Number(frame), mask };
});
const padButtonsAt = (frame) => padScript.reduce((bits, press) =>
  (frame >= press.frame && frame < press.frame + PAD_HOLD ? bits | press.mask : bits), 0);
let hostFrames = 0;
const stickScript = (option('--stick', '') || '').split(';').filter(Boolean).map((entry) => {
  const [range, values] = entry.split(':');
  const [from, to] = range.split('-').map(Number);
  const [lx, ly] = values.split(',').map(Number);
  return { from, to, lx, ly };
});
// The GS backend (browser/gs-renderer.mjs): every upload and VIF1 / GIF chain the game sends.
// --png "frame:path[,frame:path]" writes the displayed frame buffer (DISPFB2, else DISPFB1, from
// the GS privileged registers the game wrote) after that host frame.
// The WebAssembly rasterizer (make browser-gs) when built; GS_JS=1 keeps the JavaScript reference.
{
  const nativePath = join(root, 'build/browser/gs-raster.wasm');
  if (!process.env.GS_JS && existsSync(nativePath)) GsRenderer.loadNativeSync(readFileSync(nativePath));
}
const gs = new GsRenderer();
if (process.env.GS_TRACE) { gs.trace = []; gs.captureTbp = Number(process.env.GS_CAPTURE_TBP ?? 0); }
const gsStats = { chains: 0, direct: 0, vu1Calls: 0, microcode: 0, errors: 0 };
const pngShots = (option('--png', '') || '').split(',').filter(Boolean).map((entry) => {
  const at = entry.indexOf(':');
  return { frame: Number(entry.slice(0, at)), path: entry.slice(at + 1) };
});
function gsLoadImage(dbp, dbw, psm, x, y, w, h, source) {
  const bpp = { 0: 32, 1: 24, 2: 16, 10: 16, 19: 8, 20: 4, 27: 8, 36: 4, 44: 4 }[psm] ?? 32;
  const bytes = context.bytes().subarray(source, source + Math.ceil(w * h * bpp / 8));
  gs.write(0x50, (BigInt(dbp) << 32n) | (BigInt(dbw) << 48n) | (BigInt(psm) << 56n));
  gs.write(0x51, (BigInt(x) << 32n) | (BigInt(y) << 48n));
  gs.write(0x52, BigInt(w) | (BigInt(h) << 32n));
  gs.write(0x53, 0n);
  gs.image(bytes);
}
function gsDmaChain(channel, chain) {
  gsStats.chains += 1;
  try {
    if (channel === 0x1000A000) { gs.gif(readChain(context.bytes(), chain, { tte: false })); return; }
    for (const record of parseVif(readChain(context.bytes(), chain))) {
      if (record.name === 'DIRECT' || record.name === 'DIRECTHL') { gsStats.direct += 1; gs.gif(record.data); }
      else if (record.name === 'MSCAL' || record.name === 'MSCALF' || record.name === 'MSCNT') gsStats.vu1Calls += 1;
      else if (record.name === 'MPG') gsStats.microcode += 1;
    }
  } catch (error) {
    gsStats.errors += 1;
    if (gsStats.errors < 5) console.log(`GS backend: chain 0x${chain.toString(16)} not drawn (${error.message})`);
  }
}
function writePng(path, rgba, width, height) {
  const raw = Buffer.alloc((width * 4 + 1) * height);
  for (let y = 0; y < height; y++) { raw[y * (width * 4 + 1)] = 0; Buffer.from(rgba.buffer, rgba.byteOffset + y * width * 4, width * 4).copy(raw, y * (width * 4 + 1) + 1); }
  const crcTable = Array.from({ length: 256 }, (_, n) => { let c = n; for (let k = 0; k < 8; k++) c = c & 1 ? 0xEDB88320 ^ (c >>> 1) : c >>> 1; return c >>> 0; });
  const crc = (buf) => { let c = 0xFFFFFFFF; for (const b of buf) c = crcTable[(c ^ b) & 255] ^ (c >>> 8); return (c ^ 0xFFFFFFFF) >>> 0; };
  const chunk = (type, data) => { const len = Buffer.alloc(4); len.writeUInt32BE(data.length); const td = Buffer.concat([Buffer.from(type), data]); const c = Buffer.alloc(4); c.writeUInt32BE(crc(td)); return Buffer.concat([len, td, c]); };
  const ihdr = Buffer.alloc(13); ihdr.writeUInt32BE(width, 0); ihdr.writeUInt32BE(height, 4); ihdr[8] = 8; ihdr[9] = 6;
  writeFileSync(path, Buffer.concat([Buffer.from([0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A]), chunk('IHDR', ihdr), chunk('IDAT', deflateSync(raw)), chunk('IEND', Buffer.alloc(0))]));
}
function gsSnapshot(frame) {
  for (const shot of pngShots) {
    if (shot.frame !== frame) continue;
    const view = context.view();
    const dispfb2 = view.getBigUint64(0x12000090, true), dispfb1 = view.getBigUint64(0x12000070, true);
    const dispfb = dispfb2 || dispfb1;
    writePng(shot.path, gs.framebuffer(dispfb, 512, 416), 512, 416);
    if (process.env.GS_TRACE) { // the GS state, for offline diagnosis
      writeFileSync(`${shot.path}.gsmem`, gs.memory);
      if (gs.captured) { const c = gs.captured; const alpha = new Uint8Array(c.rgba); for (let i = 0; i < alpha.length; i += 4) { const a2 = Math.min(255, alpha[i + 3] * 2); alpha[i] = alpha[i + 1] = alpha[i + 2] = a2; alpha[i + 3] = 255; } writePng(`${shot.path}.captured.png`, alpha, c.width, c.height); writePng(`${shot.path}.captured-rgb.png`, new Uint8Array(c.rgba).map((v, i) => (i % 4 === 3 ? 255 : v)), c.width, c.height); console.log('captured texture palette', c.clut.join(' ')); }
      writeFileSync(`${shot.path}.trace.json`, JSON.stringify({ dispfb: dispfb.toString(16), trace: gs.trace, uploads: gs.uploadLog, regs: Object.fromEntries([...gs.reg].map(([k, v]) => [k, v.toString(16)])) }));
    }
    console.log(`GS frame ${frame} -> ${shot.path} (DISPFB 0x${dispfb.toString(16)}; ${gs.stats.primitives} primitives, ${gs.stats.uploads} uploads so far)`);
  }
}
const moviesPlayed = [];
function frameYield() {
  if (frameMode === 'off') return;
  if (frameMode !== 'rewinding') { hostFrames += 1; gsSnapshot(hostFrames); }
  if (process.env.SPIKE_YIELD_STACK && frameMode === 'running') {
    lastYieldStack = (new Error().stack ?? '').split('\n').filter((l) => l.includes('wasm')).slice(0, 10)
      .map((l) => l.trim().replace(/\s*\(wasm:.*$/, '').replace(/^at /, '')).join(' <- ');
  }
  if (frameMode === 'rewinding') { context.exports.asyncify_stop_rewind(); frameMode = 'running'; return; }
  const view = context.view();
  view.setInt32(ASYNCIFY_DATA, ASYNCIFY_DATA + 8, true);
  view.setInt32(ASYNCIFY_DATA + 4, ASYNCIFY_END, true);
  context.exports.asyncify_start_unwind(ASYNCIFY_DATA);
  frameMode = 'unwinding';
}
// Runs exports[symbol](...args) for up to `frames` frames; returns { frames, returned, result }.
function runFrames(symbol, args, frames, onFrame) {
  let done = 0, fresh = true;
  for (; done < frames; done += 1) {
    if (!fresh) { context.exports.asyncify_start_rewind(ASYNCIFY_DATA); frameMode = 'rewinding'; }
    else frameMode = 'running';
    fresh = false;
    let result;
    try {
      result = instance.exports[symbol](...args);
    } catch (error) {
      if (!(error instanceof Handover)) throw error;
      symbol = handOver(parseInt(error.message.match(/0x([0-9a-f]+)/)[1], 16));
      args = [];
      console.log(`host frame ${hostFrames}: ${error.message}; running ${symbol} in build/spike/${handoverName}.wasm`);
      fresh = true; done -= 1; frameMode = 'off';
      continue;
    }
    if (frameMode === 'unwinding') { context.exports.asyncify_stop_unwind(); frameMode = 'off'; onFrame?.(done + 1); continue; }
    frameMode = 'off';
    return { frames: done + 1, returned: true, result };
  }
  return { frames: done, returned: false };
}
let instance = new WebAssembly.Instance(module, imports);
context.memory = instance.exports.memory;
context.exports = instance.exports;
const ramBase = instance.exports.ee_ram.value;
if (ramBase !== 0x100000) throw new Error(`RAM image at 0x${ramBase.toString(16)}, expected 0x100000`);
if (instance.exports.__stack_pointer.value > 0xF0000) throw new Error('wasm stack overlaps the scratchpad mapping at 0xF0000');

if (elfPath) { // copy the boot ELF's loadable segments to their original addresses
  const elf = readFileSync(elfPath);
  const elfView = new DataView(elf.buffer, elf.byteOffset, elf.byteLength);
  const phoff = elfView.getUint32(0x1C, true);
  const phnum = elfView.getUint16(0x2C, true);
  let copied = 0;
  for (let index = 0; index < phnum; index += 1) {
    const at = phoff + index * 0x20;
    if (elfView.getUint32(at, true) !== 1) continue; // PT_LOAD
    const offset = elfView.getUint32(at + 4, true);
    const address = elfView.getUint32(at + 8, true);
    const size = elfView.getUint32(at + 0x10, true);
    context.bytes().set(elf.subarray(offset, offset + size), address);
    copied += size;
  }
  console.log(`copied ${copied} bytes of boot ELF segments to their original addresses`);
}

// Copies level N's program data over RAM, as the console's level-program load does. A scenario
// calls this again (context.loadLevelProgram) at the point the console loads the level program:
// boot code that ran before it shares these addresses, and its writes there do not survive.
const levelOption = option('--level', null);
context.loadLevelProgram = () => {
  if (levelOption === null) throw new Error('loadLevelProgram needs --level N');
  const folder = join(root, 'config/us/overlays', `level_${String(Number(levelOption)).padStart(2, '0')}`);
  const manifest = JSON.parse(readFileSync(join(folder, 'manifest.json'), 'utf8'));
  let copied = 0;
  for (const record of manifest.records) {
    if (record.name === 'text') continue; // code runs from the compiled C
    if (record.name === 'bss' || record.type !== 1) {
      context.bytes().fill(0, record.address, record.address + record.bytes);
      continue;
    }
    const data = readFileSync(join(folder, `${record.name}.bin`));
    context.bytes().set(data, record.address);
    copied += data.length;
  }
  console.log(`copied ${copied} bytes of level ${Number(levelOption)} program data to their original addresses`);
};
if (levelOption !== null) context.loadLevelProgram();

// Scratch objects in RAM the game image does not use on this path.
const moby = 0x01E00000;
const pvars = 0x01E01000;
context.view().setUint32(moby + 0x78, pvars, true);

for (let index = 0; index < process.argv.length; index += 1) { // explicit test state, e.g. hero HP
  if (process.argv[index] !== '--poke32') continue;
  const [address, value] = process.argv[index + 1].split('=').map(Number);
  context.view().setInt32(address, value, true);
  console.log(`poke32 0x${address.toString(16)} = ${value}`);
}

// Source lines for the innermost wasm frames, from the DWARF the spike objects carry. V8 reports
// byte offsets into the module file, which is what llvm-symbolizer takes for a wasm file.
function symbolize(error) {
  const offsets = [...(error.stack ?? '').matchAll(/wasm-function\[\d+\]:0x([0-9a-f]+)/g)].slice(0, 4).map((m) => m[1]);
  if (!offsets.length) return [];
  try {
    const llvm = execFileSync('em-config', ['LLVM_ROOT'], { encoding: 'utf8' }).trim();
    const text = execFileSync(join(llvm, 'llvm-symbolizer'), ['--obj', join(root, 'build/spike', `${handedOver ? handoverName : name}.wasm`),
      '--functions=none', '--basenames', ...offsets.map((offset) => `0x${offset}`)], { encoding: 'utf8' });
    return text.split('\n').filter((line) => line.trim() && !line.startsWith('??'));
  } catch {
    return [];
  }
}

const staged = await scenario.setup?.(context);
if (staged?.args) rootArgs = staged.args;

const readMoby = () => {
  const view = context.view();
  return { state: view.getUint8(moby + 0x20), flags: view.getUint16(moby + 0x34, true), alpha: view.getUint8(moby + 0x30) };
};
console.log(`root ${rootSymbol}: ${trapCount} missing functions are named traps`);
const results = [];
const sequence = staged?.sequence
  ?? Array.from({ length: calls }, () => ({ symbol: rootSymbol, args: rootArgs }));
for (let call = 1; call <= sequence.length; call += 1) {
  const { symbol, args, host, label } = sequence[call - 1];
  try {
    if (host) { host(context); console.log(`step ${call}: ${label}`); continue; }
    if (sequence[call - 1].frames) {
      const run = runFrames(symbol, args ?? [], sequence[call - 1].frames, sequence[call - 1].onFrame);
      results.push(run);
      console.log(`call ${call} ${symbol}: ${run.returned ? `returned ${run.result} after` : 'still running after'} ${run.frames} frame(s)`);
      continue;
    }
    const result = instance.exports[symbol](...(args ?? [moby]));
    results.push(result);
    console.log(args ? `call ${call} ${symbol}(${args.join(', ')}): returned ${result}` : `call ${call}: returned; moby ${JSON.stringify(readMoby())}`);
  } catch (error) {
    const where = (error.stack ?? '').split('\n').filter((line) => line.includes('wasm-function') || line.includes('wasm://'))
      .slice(0, 8).map((line) => line.trim().replace(/\s*\(wasm:.*$/, '').replace(/^at /, '')).join(' <- ');
    console.log(`call ${call} ${symbol}: stopped: ${error.message}`);
    if (where) console.log(`  in ${where}`);
    for (const line of symbolize(error)) console.log(`  at ${line}`);
    results.push(error);
    break;
  }
}
if (lastYieldStack) console.log(`last frame yielded in: ${lastYieldStack}`);
if (gsStats.chains) {
  console.log(`GS backend: ${gsStats.chains} chains, ${gsStats.direct} DIRECT packets, ${gsStats.vu1Calls} VU1 program starts (drawn by VU1 microcode: not here), ${gsStats.microcode} microcode uploads; ${gs.stats.primitives} primitives, ${gs.stats.pixels} pixels, ${gs.stats.uploads} uploads`);
  if (gs.trace) { const kinds = new Map(); for (const t of gs.trace) { const k = `type ${t.type} tme ${t.tme} psm 0x${t.psm.toString(16)}`; kinds.set(k, (kinds.get(k) ?? 0) + 1); } console.log('GS last primitives:', JSON.stringify(Object.fromEntries(kinds))); console.log('GS sample:', JSON.stringify(gs.trace.filter((t) => t.psm === 0x14 || t.psm === 0x24 || t.psm === 0x2c).slice(-4))); console.log('GS tail:', JSON.stringify(gs.trace.slice(-6))); }
  console.log(`GS texture formats sampled: ${JSON.stringify(Object.fromEntries(gs.stats.formats))}`);
  if (gs.stats.unsupported.size) console.log(`GS not implemented: ${JSON.stringify(Object.fromEntries(gs.stats.unsupported))}`);
}
scenario.report?.(context, results);
if (deferredCalls.size) {
  console.log('NOT DONE - skipped because the subsystem that consumes it does not exist yet:');
  for (const [symbol, count] of deferredCalls) console.log(`  ${symbol} x${count}: ${deferred[symbol]}`);
}
