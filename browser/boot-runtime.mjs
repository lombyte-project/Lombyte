// Runs the game's boot program (build: browser/spike/link-closure.sh FUN_0012d9d8 boot) in the
// browser: main, startlevel, the intro movie request, the title and the main menu, all original
// code compiled to WebAssembly, with the PS2's services provided here.
//
// What the host provides (the same contract as browser/spike/run-closure.mjs, which runs it in
// Node):
//   - the boot ELF's loadable segments, copied to their EE addresses before main runs;
//   - the disc: host_disc_read(sector, count, destination), read from the user's disc image;
//   - vertical sync: spike_vsync_yield, where the frame is shown and the next one waits for the
//     browser's animation frame;
//   - the GS backend (gs-renderer.mjs): every upload and VIF1 / GIF DMA chain the game sends;
//   - the controller: spike_pad_buttons, the held DualShock buttons (game bit order);
//   - fast_decompress / decompress_wad: the WAD decoder module (planet-wad.wasm);
//   - movies: spike_play_movie(sector, bytes), reported to the page (not decoded yet);
//   - the jump into a loaded program: when New Game loads Veldin, run_game_main_loop's
//     parse_bin copies the level program in and FlushCache(2) (spike_program_loaded) follows.
//     The level program runs from its own module (level-program.wasm, link-closure.sh ... level):
//     EE RAM, the scratchpad and the register pages move into it and its entry runs (handOver).
// Disc reads and vertical syncs suspend the WebAssembly call with Asyncify (link-closure.sh lists
// both imports) and resume it when the host is done.

import { deferredFunctions } from './deferred-functions.mjs';
import { GsRenderer } from './gs-renderer.mjs';
import { demuxPss } from './pss.mjs';
import { decodePs2Adpcm } from './ps2-adpcm.mjs';
import { parseVif, readChain } from './spike/display-list.mjs';

const ASYNCIFY_DATA = 0x90000, ASYNCIFY_END = 0xE0000;
const SECTOR_BYTES = 2048;
const MAIN = 'FUN_0012d9d8'; // run_game_main_loop, what crt0 calls
// Level programs built for the browser: entry address (the first chunk id parse_bin returns,
// config/us/overlays/level_NN/manifest.json entry_point) -> module and entry symbol.
const LEVEL_PROGRAMS = { 0x245c28: { url: './level-program.wasm', symbol: 'FUN_L00_00245c28', level: 0 } };
const LEVEL_ROOT = 0x15EE4C; // level_root, D_0015EE4C: the loaded level file
const LEVEL_LOAD = 0x173E00; // the level program's buffer descriptor (D_L00_00173E00)
class Handover extends Error {}

export class BootRuntime {
  constructor({ module, reader, decoder, elf, padButtons, padSticks, onFrame, onMovie, onMovieFrame, onProgram, mpeg2, audioContext, loadWorld, log }) {
    this.loadWorld = loadWorld ?? null; // async () => WorldRenderer, started when a level program takes over
    this.world = null;
    this.worldPasses = 0;
    this.onMovieFrame = onMovieFrame ?? (() => {});
    this.mpeg2 = mpeg2 ?? null; // mpeg2.wasm exports
    this.audioContext = audioContext ?? null;
    this.onProgram = onProgram ?? (() => {});
    this.entry = MAIN;
    this.program = 'boot';
    this.reader = reader;
    this.decoder = decoder;
    this.padButtons = padButtons ?? (() => 0);
    this.padSticks = padSticks ?? (() => 0x80808080);
    this.onFrame = onFrame ?? (() => {});
    this.onMovie = onMovie ?? (() => {});
    this.log = log ?? ((text) => console.log(text));
    this.gs = new GsRenderer();
    this.frames = 0;
    this.state = 'off'; // off | running | unwinding | rewinding
    this.pending = null; // { kind, promise } while unwound
    this.resumeValue = 0;
    this.deferred = new Map();
    this.imports = this.importsFor(module);
    this.module = module;
    this.elf = elf;
  }

  importsFor(module) {
    const imports = { env: {} };
    for (const { module: from, name, kind } of WebAssembly.Module.imports(module)) {
      if (kind !== 'function') continue;
      imports[from] ??= {};
      imports[from][name] = this.importFor(name);
    }
    return imports;
  }

  // Instantiates the module (asynchronously: browsers refuse a synchronous instance of this size
  // on the main thread) and copies the ELF in.
  static async create(options) {
    const runtime = new BootRuntime(options);
    runtime.instance = await WebAssembly.instantiate(runtime.module, runtime.imports);
    runtime.start();
    return runtime;
  }

  start() {
    this.exports = this.instance.exports;
    this.memory = this.exports.memory;
    if (this.exports.ee_ram.value !== 0x100000) throw new Error('boot.wasm: RAM image is not at 0x100000');
    const grow = (0x12002000 - this.memory.buffer.byteLength) / 65536;
    if (grow > 0) this.memory.grow(Math.ceil(grow));
    this.copyElf(this.elf);
  }

  bytes() { return new Uint8Array(this.memory.buffer); }
  view() { return new DataView(this.memory.buffer); }
  cString(at) { const b = this.bytes(); let end = at; while (b[end]) end++; return new TextDecoder().decode(b.subarray(at, end)); }

  copyElf(elf) {
    const view = new DataView(elf.buffer, elf.byteOffset, elf.byteLength);
    const phoff = view.getUint32(0x1C, true), phnum = view.getUint16(0x2C, true);
    for (let i = 0; i < phnum; i++) {
      const at = phoff + i * 0x20;
      if (view.getUint32(at, true) !== 1) continue;
      const offset = view.getUint32(at + 4, true), address = view.getUint32(at + 8, true), size = view.getUint32(at + 0x10, true);
      this.bytes().set(elf.subarray(offset, offset + size), address);
    }
    // The sky's two scratchpad buffers are console scratchpad addresses held in data
    // (D_00160408); the scratchpad is at 0xF0000 in this build.
    const v = this.view();
    for (const at of [0x160408, 0x16040C]) {
      const value = v.getUint32(at, true);
      if ((value & 0xFFFFC000) === 0x70000000) v.setUint32(at, 0xF0000 + (value & 0x3FFF), true);
    }
  }

  // Suspends the current WebAssembly call until `promise` settles; its value is what the import
  // returns when the call resumes.
  suspend(kind, promise) {
    if (this.state === 'rewinding') {
      this.exports.asyncify_stop_rewind();
      this.state = 'running';
      return this.resumeValue;
    }
    const v = this.view();
    v.setInt32(ASYNCIFY_DATA, ASYNCIFY_DATA + 8, true);
    v.setInt32(ASYNCIFY_DATA + 4, ASYNCIFY_END, true);
    this.pending = { kind, promise: promise() };
    this.exports.asyncify_start_unwind(ASYNCIFY_DATA);
    this.state = 'unwinding';
    return 0;
  }

  importFor(name) {
    switch (name) {
      case 'spike_vsync_yield': return () => this.suspend('vsync', () => this.vsync());
      case 'host_disc_read': return (sector, count, destination) => this.suspend('disc', () => this.discRead(sector, count, destination));
      case 'spike_pad_buttons': return () => this.padButtons() & 0xFFFF;
      case 'spike_pad_sticks': return () => this.padSticks() >>> 0;
      case 'spike_host_frame': return () => this.frames;
      case 'spike_read_clock': return (at) => { const now = new Date(); const bcd = (n) => ((Math.floor(n / 10) << 4) | (n % 10)); const b = this.bytes(); b.set([0, bcd(now.getSeconds()), bcd(now.getMinutes()), bcd(now.getHours()), 0, bcd(now.getDate()), bcd(now.getMonth() + 1), bcd(now.getFullYear() % 100)], at); };
      case 'spike_gs_load_image': return (dbp, dbw, psm, x, y, w, h, source) => this.gsLoadImage(dbp, dbw, psm, x, y, w, h, source);
      case 'spike_dma_chain': return (channel, chain) => this.gsDmaChain(channel >>> 0, chain >>> 0);
      case 'spike_play_movie': return (sector, bytes, channel) => this.suspend('movie', () => this.playMovie(sector, bytes, channel));
      case 'spike_debug_print': return (at) => this.log(`game: ${this.cString(at).replace(/\n$/, '')}`);
      case 'spike_deferred': return (nameAt, reasonAt) => { const key = this.cString(nameAt); this.deferred.set(key, (this.deferred.get(key) ?? 0) + 1); };
      case 'spike_unimplemented_rpc': return (server, fn) => { throw new Error(`IOP RPC server 0x${(server >>> 0).toString(16)} is not implemented (function ${fn})`); };
      case 'spike_world_pass': return () => {
        // services.c tfrag_proc: the level's world, drawn by the browser renderer into this frame.
        if (!this.world || this.program === 'boot') return;
        this.worldPasses += 1;
        if (this.trace) this.trace.push('W');
        this.gs.phase = 'after-world';
        const e = this.exports;
        const matrices = e.spike_moby_matrices ? {
          count: (moby) => e.spike_moby_matrices(moby),
          matrix: (j) => new Float32Array(this.memory.buffer, e.spike_moby_matrix_buffer() + 64 * j, 16),
        } : null;
        if (this.world.display) {
          // Layered display: the frame so far is the background; the world is drawn on its own
          // canvas at the page's resolution; what the game draws after this is the foreground.
          // The game draws each frame in one buffer and its final pass (the softening copy) puts it
          // in the displayed one, so the snapshot is of the drawing buffer.
          this.background = { ...this.gs.frameBufferRgba(this.world.width, this.world.height), frame: this.frames };
          if (this.showDrawBuffer) this.gs.displaySourceTbp = this.background.fbp * 32;
          this.world.drawDisplay(this.view(), this.gs.reg.get(0x18) ?? 0n, matrices);
          // The world's depth in GS units, written into the Z buffer where the next frame's draw
          // list reaches its world (below), so the game's 3D effects are hidden behind terrain.
          if (this.depthTest !== false) {
            const packed = this.world.gsDepth(this.view());
            if (packed) this.worldDepth = { packed, frame: this.frames };
          }
        } else {
          this.gs.blitWorld(this.world.render(this.view(), this.gs.reg.get(0x18) ?? 0n, matrices), this.world.width, this.world.height);
        }
      };
      case 'spike_program_loaded': return () => { const entry = this.loadedProgramEntry(); if (entry) throw new Handover(String(entry)); };
      case 'spike_conflict': return (at) => { throw new Error(`missing function with conflicting caller prototypes: ${this.cString(at)}`); };
      case 'FUN_0020b618': case 'FUN_L00_0024d8d0': return (source, destination) => this.decompress(source, destination);
      default:
        if (deferredFunctions[name]) return () => { this.deferred.set(name, (this.deferred.get(name) ?? 0) + 1); return 0; };
        return () => { throw new Error(`missing function: ${name}`); };
    }
  }

  async discRead(sector, count, destination) {
    const CHUNK = 512; // sectors: IsoReader serves at most 1 MiB per request
    for (let done = 0; done < count; done += CHUNK) {
      const n = Math.min(CHUNK, count - done);
      const data = await this.reader.readSectors(sector + done, n);
      this.bytes().set(data.subarray(0, n * SECTOR_BYTES), destination + done * SECTOR_BYTES);
    }
    return 1;
  }

  // play_mpeg_movie as a browser service: the PSS file at that disc extent, its video decoded by
  // mpeg2.wasm and shown at its frame rate against the audio clock, the chosen audio track (PS2
  // ADPCM) through Web Audio. Returns when the movie ends or START skips it (ASSUMED: skipping is
  // a convenience here; the original player's handling of the pad is not reproduced).
  async playMovie(sector, bytes, channel) {
    this.onMovie(sector, bytes, channel);
    if (!this.mpeg2) return 0;
    const sectors = Math.ceil(bytes / SECTOR_BYTES);
    const file = new Uint8Array(sectors * SECTOR_BYTES);
    for (let done = 0; done < sectors; done += 512) {
      const n = Math.min(512, sectors - done);
      file.set((await this.reader.readSectors(sector + done, n)).subarray(0, n * SECTOR_BYTES), done * SECTOR_BYTES);
    }
    const { video, audio } = demuxPss(file.subarray(0, bytes), channel);
    const m = this.mpeg2;
    new Uint8Array(m.memory.buffer, m.mpeg2_input(), video.length).set(video);
    const info = m.mpeg2_start(video.length);
    if (info < 0) { this.log(`movie at sector ${sector}: decoder could not start (${info})`); return 0; }
    const width = info >> 16, height = info & 0xFFFF, fps = 30;
    let source = null, startTime = performance.now() / 1000, clock = () => performance.now() / 1000 - startTime;
    if (audio && this.audioContext && audio.type === 16) {
      const ctx = this.audioContext;
      // resume() stays pending until the browser allows audio (a user gesture): never wait on it
      // for long. A movie whose audio cannot start yet plays silently on the page clock.
      await Promise.race([ctx.resume().catch(() => {}), new Promise((resolve) => setTimeout(resolve, 250))]);
      const channels = decodePs2Adpcm(audio.data, audio.channels, audio.interleave);
      const buffer = ctx.createBuffer(channels.length, channels[0].length, audio.rate);
      channels.forEach((samples, i) => buffer.copyToChannel(samples, i));
      source = ctx.createBufferSource();
      source.buffer = buffer;
      source.connect(ctx.destination);
      const t0 = ctx.currentTime + 0.05;
      source.start(t0);
      if (ctx.state === 'running') clock = () => ctx.currentTime - t0;
      else this.log('movie audio is waiting for a key press or click (the browser blocks sound until then)');
    }
    const startButtons = this.padButtons();
    let shown = 0, ended = false;
    while (!ended) {
      await new Promise((resolve) => requestAnimationFrame(resolve));
      const buttons = this.padButtons();
      if ((buttons & 0x0800) && !(startButtons & 0x0800)) break; // START skips
      const due = Math.floor(Math.max(0, clock()) * fps);
      let frame = null;
      while (shown <= due) {
        const r = m.mpeg2_next();
        if (r <= 0) { ended = true; break; }
        shown += 1;
        frame = r;
      }
      if (frame) this.onMovieFrame(new Uint8Array(m.memory.buffer, m.mpeg2_output(), width * height * 4), width, height);
    }
    if (source) { try { source.stop(); } catch { /* already ended */ } }
    return 0;
  }

  vsync() {
    this.frames += 1;
    this.onFrame(this);
    if (this.trace) { this.traces = [...(this.traces ?? []).slice(-3), this.trace.filter(Boolean).join('').replace(/D+/g, (m) => `D${m.length}`).replace(/M+/g, (m) => `M${m.length}`)]; this.trace = []; }
    this.gs.phase = 'before-world';
    this.gs.afterSky = false;
    return new Promise((resolve) => requestAnimationFrame(() => resolve(0)));
  }

  // The program parse_bin has just copied in (boot program only): level_root's first chunk
  // { dst, size, pad, id } is at its destination; its id is the entry address.
  loadedProgramEntry() {
    if (this.program !== 'boot') return 0;
    const view = this.view(), bytes = this.bytes();
    const rootAt = view.getUint32(LEVEL_ROOT, true);
    if (!rootAt || rootAt >= 0x2000000) return 0;
    const chunk = rootAt + view.getUint32(rootAt, true);
    const dst = view.getUint32(chunk, true), size = view.getInt32(chunk + 4, true), id = view.getUint32(chunk + 12, true);
    if (!dst || dst >= 0x2000000 || size <= 0 || size > 0x2000000) return 0;
    for (let i = 0, n = Math.min(size, 4096); i < n; i++) if (bytes[dst + i] !== bytes[chunk + 16 + i]) return 0;
    return id;
  }

  // The console jumps to the loaded program. Its module takes over: EE RAM, the scratchpad and the
  // EE / GS register pages are copied in (each module's C statics sit above 0x2000000 and start
  // fresh), the loaded handler tables get the compiled C tables, the level copy of the sky's
  // scratchpad pointers is translated, and the entry runs from the next frame.
  async handOver(entry) {
    const program = LEVEL_PROGRAMS[entry];
    if (!program) throw new Error(`the game loaded a level program with entry 0x${entry.toString(16)}, which is not built for the browser`);
    this.onProgram(program);
    const module = await WebAssembly.compileStreaming(fetch(program.url, { cache: 'no-cache' }));
    const instance = await WebAssembly.instantiate(module, this.importsFor(module));
    const previousVblankRuns = this.exports.spike_vblank_runs?.() ?? 0;
    const previousCdCallback = this.exports.spike_cd_callback_installed?.() ?? 0;
    const from = this.bytes();
    const memory = instance.exports.memory;
    if (memory.buffer.byteLength < from.length) memory.grow(Math.ceil((from.length - memory.buffer.byteLength) / 65536));
    const to = new Uint8Array(memory.buffer);
    for (const [start, end] of [[0xF0000, 0xF4000], [0x100000, 0x2000000], [0x10000000, 0x10010000], [0x12000000, 0x12002000]]) to.set(from.subarray(start, end), start);
    this.instance = instance;
    this.exports = instance.exports;
    this.memory = memory;
    this.program = `level ${program.level}`;
    if (this.loadWorld && program.level === 0) {
      this.loadWorld().then((world) => { this.world = world; this.log(`world renderer ready: ${Math.round(world.triangles)} triangles`); })
        .catch((error) => this.log(`world renderer could not load: ${error.message}`));
    }
    this.entry = program.symbol;
    this.exports.spike_install_level_tables();
    // The vsync interrupt handler (count_vsync: it polls the pads) keeps running across the jump.
    this.exports.spike_resume_after_handover?.(previousVblankRuns > 0 ? 1 : 0, previousCdCallback);
    const v = this.view();
    for (const at of [0x1604C8, 0x1604CC]) {
      const value = v.getUint32(at, true);
      if ((value & 0xFFFFC000) === 0x70000000) v.setUint32(at, 0xF0000 + (value & 0x3FFF), true);
    }
  }

  // The decoded level assets hold the static collision mesh; the level program's collision queries
  // read it from the collision module's own storage (as scenario-veldin-level.mjs does).
  stageCollision(destination) {
    if (this.program === 'boot' || !this.exports.runtime_collision_load) return;
    const view = this.view();
    if (destination !== view.getUint32(LEVEL_LOAD + 0x14, true)) return;
    const header = view.getUint32(LEVEL_LOAD + 0x04, true);
    const start = view.getUint32(header + 0x14, true), end = view.getUint32(header + 0x60, true);
    const e = this.exports;
    if (!(start > 0 && end > start && end - start <= e.runtime_collision_capacity())) throw new Error(`collision range ${start}-${end} is not usable`);
    e.runtime_collision_reset();
    this.bytes().copyWithin(e.runtime_collision_stage_address(), destination + start, destination + end);
    const result = e.runtime_collision_load(end - start);
    if (result < 0) throw new Error(`collision mesh rejected (${result})`);
  }

  decompress(source, destination) {
    const bytes = this.bytes(), d = this.decoder;
    if (bytes[source] !== 0x57 || bytes[source + 1] !== 0x41 || bytes[source + 2] !== 0x44) throw new Error(`fast_decompress: no WAD stream at 0x${source.toString(16)}`);
    const length = this.view().getUint32(source + 3, true);
    d.wad_reset();
    new Uint8Array(d.memory.buffer, d.wad_input_address(), length).set(bytes.subarray(source, source + length));
    const size = d.wad_decode(length, d.wad_output_capacity());
    if (size <= 0 || d.wad_ready() !== 1) throw new Error(`fast_decompress: decode failed (${size})`);
    this.bytes().set(new Uint8Array(d.memory.buffer, d.wad_output_address(), size), destination);
    d.wad_reset();
    this.stageCollision(destination);
    return size;
  }

  gsLoadImage(dbp, dbw, psm, x, y, w, h, source) {
    const bpp = { 0: 32, 1: 24, 2: 16, 10: 16, 19: 8, 20: 4, 27: 8, 36: 4, 44: 4 }[psm] ?? 32;
    const gs = this.gs;
    gs.write(0x50, (BigInt(dbp) << 32n) | (BigInt(dbw) << 48n) | (BigInt(psm) << 56n));
    gs.write(0x51, (BigInt(x) << 32n) | (BigInt(y) << 48n));
    gs.write(0x52, BigInt(w) | (BigInt(h) << 32n));
    gs.write(0x53, 0n);
    gs.image(this.bytes().subarray(source, source + Math.ceil(w * h * bpp / 8)));
  }

  gsDmaChain(channel, chain) {
    if (channel === 0x1000A000) { this.gs.gif(readChain(this.bytes(), chain, { tte: false })); return; }
    for (const record of parseVif(readChain(this.bytes(), chain))) {
      if (this.trace) this.trace.push(record.name === 'DIRECT' || record.name === 'DIRECTHL' ? 'D' : /^MSC/.test(record.name) ? 'M' : '');
      // Layering (gameplay): each frame the game sends its whole draw list before its world pass:
      // the sky's GIF packets, then the first VU1 program start (where the world's geometry goes
      // on the console), then everything drawn over the world (effects, HUD). The page's sky
      // layer is the frame buffer at that first VU1 start, so what follows stays in front.
      if (/^MSC/.test(record.name) && this.world && this.program !== 'boot' && this.skyMarkFrame !== this.frames) {
        this.skyMarkFrame = this.frames;
        this.skyLayer = { ...this.gs.frameBufferRgba(this.world.width, this.world.height), frame: this.frames };
        this.gs.afterSky = true;
        if (this.worldDepth && this.frames - this.worldDepth.frame <= 4) this.gs.blitZ(this.worldDepth.packed, this.world.width, this.world.height);
      }
      if (record.name === 'DIRECT' || record.name === 'DIRECTHL') this.gs.gif(record.data);
    }
  }

  // The displayed picture: DISPFB2 (else DISPFB1) as the game set it.
  picture(width = 512, height = 416) {
    // In gameplay with showDrawBuffer the display copy is skipped (gs-renderer.mjs): the picture is
    // the draw buffer the world pass drew into, at the same size and place as the background.
    const bg = this.background;
    if (this.showDrawBuffer && bg && this.frames - bg.frame <= 2) return this.gs.framebuffer(bg.dispfb, width, height);
    this.gs.displaySourceTbp = undefined;
    const v = this.view();
    const dispfb = v.getBigUint64(0x12000090, true) || v.getBigUint64(0x12000070, true);
    return this.gs.framebuffer(dispfb, width, height);
  }

  // Runs main until it stops; each vertical sync shows a frame and waits for the browser.
  async run() {
    this.state = 'running';
    for (;;) {
      try {
        this.exports[this.entry]();
      } catch (error) {
        if (!(error instanceof Handover)) throw error;
        await this.handOver(Number(error.message));
        this.state = 'running';
        continue;
      }
      if (this.state !== 'unwinding') { this.state = 'off'; return; }
      this.exports.asyncify_stop_unwind();
      this.resumeValue = await this.pending.promise;
      this.pending = null;
      this.exports.asyncify_start_rewind(ASYNCIFY_DATA);
      this.state = 'rewinding';
    }
  }
}
