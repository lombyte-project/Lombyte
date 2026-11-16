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
//                               { sequence: [{ symbol, args }] }  to call several roots in order
//   deferred                 -> { importName: reason }     output-only work with no consumer yet;
//                                                           returns 0, is counted, and is listed
//                                                           at the end of every run as NOT DONE
//   report(context, results)                               print what the run produced
// context = { memory, view(), bytes(), exports, cString }.
import { execFileSync } from 'node:child_process';
import { readFileSync } from 'node:fs';
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
const imports = {};
let trapCount = 0;
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
  } else if (symbol === 'spike_unimplemented_rpc') {
    imports[from][symbol] = (server, fn) => { throw new Error(`IOP RPC server 0x${server.toString(16)} is not implemented (function ${fn})`); };
  } else if (symbol === 'spike_conflict') {
    imports[from][symbol] = (address) => { throw new Error(`missing function with conflicting caller prototypes: ${context.cString(address)}`); };
    trapCount += 1;
  } else {
    imports[from][symbol] = () => { throw new Error(`missing function: ${symbol}`); };
    trapCount += 1;
  }
}
const instance = new WebAssembly.Instance(module, imports);
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

const levelOption = option('--level', null);
if (levelOption !== null) {
  const folder = join(root, 'config/us/overlays', `level_${String(Number(levelOption)).padStart(2, '0')}`);
  const manifest = JSON.parse(readFileSync(join(folder, 'manifest.json'), 'utf8'));
  let copied = 0;
  for (const record of manifest.records) {
    if (record.name === 'text' || record.name === 'bss' || record.type !== 1) continue;
    const data = readFileSync(join(folder, `${record.name}.bin`));
    context.bytes().set(data, record.address);
    copied += data.length;
  }
  console.log(`copied ${copied} bytes of level ${Number(levelOption)} program data to their original addresses`);
}

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
    const text = execFileSync(join(llvm, 'llvm-symbolizer'), ['--obj', join(root, 'build/spike', `${name}.wasm`),
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
  const { symbol, args } = sequence[call - 1];
  try {
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
scenario.report?.(context, results);
if (deferredCalls.size) {
  console.log('NOT DONE - skipped because the subsystem that consumes it does not exist yet:');
  for (const [symbol, count] of deferredCalls) console.log(`  ${symbol} x${count}: ${deferred[symbol]}`);
}
