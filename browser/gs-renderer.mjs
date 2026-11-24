// A browser GS backend: draws the GIF packets the game sends to the PS2's Graphics Synthesizer.
//
// The game builds GS register writes (GIF packets) every frame and sends them through VIF1
// (DIRECT) and image uploads (sceGsExecLoadImage). This module keeps the GS's 4 MiB local memory
// and register state and draws into that memory, so frame buffers, Z buffers, textures and
// palettes live where the game put them, in the GS's own pixel layouts (page/block/column
// swizzles). The displayed picture is read back from the frame buffer the game selects
// (DISPFB1/2, privileged registers, read by the caller).
//
// Not a CPU or VU emulator: only the GS's documented register interface (GS User's Manual) is
// implemented, in software. VU1 programs' output is not seen here; the browser renderer draws that
// geometry. Implemented: PACKED / REGLIST / IMAGE GIF tags; PRIM, RGBAQ, ST, UV, XYZF2/3, XYZ2/3,
// TEX0_1/2, CLAMP_1/2, FOG, XYOFFSET_1/2, PRMODECONT, PRMODE, TEX1, TEX2, SCISSOR, ALPHA, TEST,
// FBA, FRAME, ZBUF, BITBLTBUF, TRXPOS, TRXREG, TRXDIR, TEXA, TEXCLUT; points, lines, line strips,
// triangles, strips, fans and sprites; Gouraud or flat colour; textures (PSMCT32/24/16, PSMT8,
// PSMT4, PSMT8H/4HL/4HH) with CSM1 palettes, nearest or bilinear sampling (TEX1 MMAG / MMIN on
// mipmap level 0); TFX modulate/decal/highlight; alpha blending (A, B, C, D), alpha test, Z test
// and write, scissor. Not yet: mipmap levels, fog blending, dithering, frame-buffer masks other than whole-pixel, CSM2.

const MEMORY_BYTES = 4 * 1024 * 1024;

// Pixel layouts (GS User's Manual, "Local memory"): block order within a page and pixel order
// within a block, for each storage format.
const BLOCK32 = [[0, 1, 4, 5, 16, 17, 20, 21], [2, 3, 6, 7, 18, 19, 22, 23], [8, 9, 12, 13, 24, 25, 28, 29], [10, 11, 14, 15, 26, 27, 30, 31]];
const COLUMN32 = [[0, 1, 4, 5, 8, 9, 12, 13], [2, 3, 6, 7, 10, 11, 14, 15], [16, 17, 20, 21, 24, 25, 28, 29], [18, 19, 22, 23, 26, 27, 30, 31],
  [32, 33, 36, 37, 40, 41, 44, 45], [34, 35, 38, 39, 42, 43, 46, 47], [48, 49, 52, 53, 56, 57, 60, 61], [50, 51, 54, 55, 58, 59, 62, 63]];
const BLOCK16 = [[0, 2, 8, 10], [1, 3, 9, 11], [4, 6, 12, 14], [5, 7, 13, 15], [16, 18, 24, 26], [17, 19, 25, 27], [20, 22, 28, 30], [21, 23, 29, 31]];
const COLUMN16 = [
  [0, 2, 8, 10, 16, 18, 24, 26, 1, 3, 9, 11, 17, 19, 25, 27], [4, 6, 12, 14, 20, 22, 28, 30, 5, 7, 13, 15, 21, 23, 29, 31],
  [32, 34, 40, 42, 48, 50, 56, 58, 33, 35, 41, 43, 49, 51, 57, 59], [36, 38, 44, 46, 52, 54, 60, 62, 37, 39, 45, 47, 53, 55, 61, 63],
  [64, 66, 72, 74, 80, 82, 88, 90, 65, 67, 73, 75, 81, 83, 89, 91], [68, 70, 76, 78, 84, 86, 92, 94, 69, 71, 77, 79, 85, 87, 93, 95],
  [96, 98, 104, 106, 112, 114, 120, 122, 97, 99, 105, 107, 113, 115, 121, 123], [100, 102, 108, 110, 116, 118, 124, 126, 101, 103, 109, 111, 117, 119, 125, 127]];
const BLOCK8 = BLOCK32;
const COLUMN8 = [
  [0, 4, 16, 20, 32, 36, 48, 52, 2, 6, 18, 22, 34, 38, 50, 54], [8, 12, 24, 28, 40, 44, 56, 60, 10, 14, 26, 30, 42, 46, 58, 62],
  [33, 37, 49, 53, 1, 5, 17, 21, 35, 39, 51, 55, 3, 7, 19, 23], [41, 45, 57, 61, 9, 13, 25, 29, 43, 47, 59, 63, 11, 15, 27, 31],
  [96, 100, 112, 116, 64, 68, 80, 84, 98, 102, 114, 118, 66, 70, 82, 86], [104, 108, 120, 124, 72, 76, 88, 92, 106, 110, 122, 126, 74, 78, 90, 94],
  [65, 69, 81, 85, 97, 101, 113, 117, 67, 71, 83, 87, 99, 103, 115, 119], [73, 77, 89, 93, 105, 109, 121, 125, 75, 79, 91, 95, 107, 111, 123, 127],
  [128, 132, 144, 148, 160, 164, 176, 180, 130, 134, 146, 150, 162, 166, 178, 182], [136, 140, 152, 156, 168, 172, 184, 188, 138, 142, 154, 158, 170, 174, 186, 190],
  [161, 165, 177, 181, 129, 133, 145, 149, 163, 167, 179, 183, 131, 135, 147, 151], [169, 173, 185, 189, 137, 141, 153, 157, 171, 175, 187, 191, 139, 143, 155, 159],
  [224, 228, 240, 244, 192, 196, 208, 212, 226, 230, 242, 246, 194, 198, 210, 214], [232, 236, 248, 252, 200, 204, 216, 220, 234, 238, 250, 254, 202, 206, 218, 222],
  [193, 197, 209, 213, 225, 229, 241, 245, 195, 199, 211, 215, 227, 231, 243, 247], [201, 205, 217, 221, 233, 237, 249, 253, 203, 207, 219, 223, 235, 239, 251, 255]];
const BLOCK4 = BLOCK16;
const COLUMN4_HALF = [
  [0, 8, 32, 40, 64, 72, 96, 104, 2, 10, 34, 42, 66, 74, 98, 106, 4, 12, 36, 44, 68, 76, 100, 108, 6, 14, 38, 46, 70, 78, 102, 110],
  [16, 24, 48, 56, 80, 88, 112, 120, 18, 26, 50, 58, 82, 90, 114, 122, 20, 28, 52, 60, 84, 92, 116, 124, 22, 30, 54, 62, 86, 94, 118, 126],
  [65, 73, 97, 105, 1, 9, 33, 41, 67, 75, 99, 107, 3, 11, 35, 43, 69, 77, 101, 109, 5, 13, 37, 45, 71, 79, 103, 111, 7, 15, 39, 47],
  [81, 89, 113, 121, 17, 25, 49, 57, 83, 91, 115, 123, 19, 27, 51, 59, 85, 93, 117, 125, 21, 29, 53, 61, 87, 95, 119, 127, 23, 31, 55, 63],
  [192, 200, 224, 232, 128, 136, 160, 168, 194, 202, 226, 234, 130, 138, 162, 170, 196, 204, 228, 236, 132, 140, 164, 172, 198, 206, 230, 238, 134, 142, 166, 174],
  [208, 216, 240, 248, 144, 152, 176, 184, 210, 218, 242, 250, 146, 154, 178, 186, 212, 220, 244, 252, 148, 156, 180, 188, 214, 222, 246, 254, 150, 158, 182, 190],
  [129, 137, 161, 169, 193, 201, 225, 233, 131, 139, 163, 171, 195, 203, 227, 235, 133, 141, 165, 173, 197, 205, 229, 237, 135, 143, 167, 175, 199, 207, 231, 239],
  [145, 153, 177, 185, 209, 217, 241, 249, 147, 155, 179, 187, 211, 219, 243, 251, 149, 157, 181, 189, 213, 221, 245, 253, 151, 159, 183, 191, 215, 223, 247, 255]];
const COLUMN4 = [...COLUMN4_HALF, ...COLUMN4_HALF.map((row) => row.map((v) => v + 256))];

const PSM = { CT32: 0x00, CT24: 0x01, CT16: 0x02, CT16S: 0x0A, T8: 0x13, T4: 0x14, T8H: 0x1B, T4HL: 0x24, T4HH: 0x2C, Z32: 0x30, Z24: 0x31, Z16: 0x32, Z16S: 0x3A };

// Byte address of pixel (x, y) for base pointer bp (in 256-byte blocks) and buffer width bw (in
// 64-pixel units), per storage format. 4-bit formats return a nibble address (byte * 2 + half).
// The block and column tables above folded into one offset per pixel of a page (the same values,
// precomputed): 32-bit pages are 64 x 32 pixels, 16-bit 64 x 64, 8-bit 128 x 64, 4-bit 128 x 128.
const SWZ32 = new Int32Array(32 * 64), SWZ16 = new Int32Array(64 * 64), SWZ8 = new Int32Array(64 * 128), SWZ4 = new Int32Array(128 * 128);
for (let y = 0; y < 32; y++) for (let x = 0; x < 64; x++) SWZ32[(y << 6) | x] = BLOCK32[(y >> 3) & 3][(x >> 3) & 7] * 64 + COLUMN32[y & 7][x & 7];
for (let y = 0; y < 64; y++) for (let x = 0; x < 64; x++) SWZ16[(y << 6) | x] = BLOCK16[(y >> 3) & 7][(x >> 4) & 3] * 128 + COLUMN16[y & 7][x & 15];
for (let y = 0; y < 64; y++) for (let x = 0; x < 128; x++) SWZ8[(y << 7) | x] = BLOCK8[(y >> 4) & 3][(x >> 4) & 7] * 256 + COLUMN8[y & 15][x & 15];
for (let y = 0; y < 128; y++) for (let x = 0; x < 128; x++) SWZ4[(y << 7) | x] = BLOCK4[(y >> 4) & 7][(x >> 5) & 3] * 512 + COLUMN4[y & 15][x & 31];
function address32(bp, bw, x, y) {
  return (bp * 64 + ((y >> 5) * bw + (x >> 6)) * 2048 + SWZ32[((y & 31) << 6) | (x & 63)]) * 4;
}
function address16(bp, bw, x, y) {
  return (bp * 128 + ((y >> 6) * bw + (x >> 6)) * 4096 + SWZ16[((y & 63) << 6) | (x & 63)]) * 2;
}
function address8(bp, bw, x, y) {
  return bp * 256 + ((y >> 6) * (bw >> 1) + (x >> 7)) * 8192 + SWZ8[((y & 63) << 7) | (x & 127)];
}
function address4(bp, bw, x, y) { // nibble address
  return bp * 512 + ((y >> 7) * (bw >> 1) + (x >> 7)) * 16384 + SWZ4[((y & 127) << 7) | (x & 127)];
}

const lo32 = (v) => Number(v & 0xFFFFFFFFn);
const bits = (v, at, width) => Number((v >> BigInt(at)) & ((1n << BigInt(width)) - 1n));

// The rasterizer in WebAssembly (browser/gs-raster.c, `make browser-gs`): the same pixel pipeline
// as the methods below, several times faster. When attached, GS memory lives in its memory and
// draws, uploads and the display read-out run there; this file's own versions stay the reference
// (GS_JS=1 in Node forces them) and keep serving everything else (registers, CLUTs, diagnosis).
const STATE_FIELDS = ['tme', 'fst', 'abe', 'iip', 'scx0', 'scx1', 'scy0', 'scy1', 'ate', 'atst', 'aref', 'afail', 'zte', 'ztst',
  'zbp', 'zpsm', 'zmsk', 'fbp', 'fbw', 'fpsm', 'fbmsk', 'asel', 'bsel', 'csel', 'dsel', 'fix', 'fba', 'pabe', 'fb16'];
const TEXTURE_FIELDS = ['tbp', 'tbw', 'psm', 'tw', 'th', 'tcc', 'tfx', 'indexed', 'wms', 'wmt', 'minu', 'maxu', 'minv', 'maxv', 'ta0',
  'lcm', 'lodL', 'mmag', 'mminLinear'];
const UPLOAD_CHUNK = 0xFFFF0; // a multiple of 2, 3 and 4 bytes, so no pixel straddles two chunks

export class GsRenderer {
  // Instantiates gs-raster.wasm from its bytes (or a compiled module) for every renderer made after.
  static async loadNative(source) {
    const module = source instanceof WebAssembly.Module ? source : await WebAssembly.compile(source);
    GsRenderer.native = (await WebAssembly.instantiate(module, {})).exports;
  }
  static loadNativeSync(bytes) {
    GsRenderer.native = new WebAssembly.Instance(new WebAssembly.Module(bytes), {}).exports;
  }

  constructor() {
    const n = GsRenderer.native;
    if (n) {
      this.native = n;
      const buffer = n.memory.buffer;
      this.memory = new Uint8Array(buffer, n.gs_memory(), MEMORY_BYTES);
      this.memory.fill(0);
      this.nativeClut = new Uint32Array(buffer, n.gs_clut(), 256);
      this.nativeState = new Int32Array(buffer, n.gs_state_address(), STATE_FIELDS.length + TEXTURE_FIELDS.length + 3);
      this.nativeLodK = new Float64Array(buffer, n.gs_lodk_address(), 1);
      this.nativeVertices = new Float64Array(buffer, n.gs_vertices_address(), 36);
      this.nativeTransfer = new Int32Array(buffer, n.gs_transfer_address(), 9);
      this.nativeUpload = new Uint8Array(buffer, n.gs_upload_buffer(), UPLOAD_CHUNK);
      [SWZ32, SWZ16, SWZ8, SWZ4].forEach((table, which) => new Int32Array(buffer, n.gs_table(which), table.length).set(table));
    } else {
      this.memory = new Uint8Array(MEMORY_BYTES);
    }
    this.view = new DataView(this.memory.buffer, this.memory.byteOffset, MEMORY_BYTES);
    this.u32 = new Uint32Array(this.memory.buffer, this.memory.byteOffset, MEMORY_BYTES / 4); // aligned 32-bit access (little-endian hosts)
    this.reg = new Map(); // register address -> BigInt value
    this.queue = []; // vertex queue for the current primitive
    this.prim = 0n;
    this.rgbaq = { r: 0x80, g: 0x80, b: 0x80, a: 0x80, q: 1 };
    this.st = { s: 0, t: 0 };
    this.uv = { u: 0, v: 0 };
    this.fog = 0;
    this.transfer = null; // host-to-local transfer in progress
    this.p = { r: 0, g: 0, b: 0, a: 0, z: 0, u: 0, vv: 0, s: 0, t: 0, q: 1 }; // the pixel being drawn (reused)
    this.stats = { packets: 0, primitives: 0, pixels: 0, uploads: 0, uploadBytes: 0, unsupported: new Map(), formats: new Map() };
    for (const [address, value] of [[0x4C, 0n], [0x4D, 0n], [0x18, 0n], [0x19, 0n], [0x40, 0n], [0x41, 0n], [0x47, 0n], [0x48, 0n]]) this.reg.set(address, value);
  }

  note(what) { this.stats.unsupported.set(what, (this.stats.unsupported.get(what) ?? 0) + 1); }

  // One GIF packet stream (as carried by VIF1 DIRECT, quadword-aligned bytes).
  // The GIF sees one continuous stream: a tag's data may continue in the next DIRECT packet (an
  // IMAGE tag whose pixels follow from a DMA ref, or a PACKED / REGLIST loop split by the game),
  // so an unfinished tag carries over between calls.
  gif(bytes) {
    const view = new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength);
    let at = 0;
    if (this.pending) at = this.resume(bytes, view);
    while (at + 16 <= bytes.length) {
      const tag = view.getBigUint64(at, true), regs = view.getBigUint64(at + 8, true);
      at += 16;
      const nloop = bits(tag, 0, 15), eop = bits(tag, 15, 1), pre = bits(tag, 46, 1);
      const flg = bits(tag, 58, 2), nreg = bits(tag, 60, 4) || 16;
      this.stats.packets += 1;
      if (pre && flg !== 2) this.write(0x00, BigInt(bits(tag, 47, 11)));
      this.pending = { flg, regs, nreg, left: flg === 2 ? nloop * 16 : nloop * nreg, index: 0, words: nloop * nreg };
      at = this.resume(bytes, view, at);
      if (eop && at >= bytes.length) break;
    }
  }

  // Continues the pending tag from `at`; returns where its data ended (or the packet's end).
  resume(bytes, view, at = 0) {
    const p = this.pending;
    if (p.flg === 2) { // IMAGE: p.left bytes of pixels
      const take = Math.min(p.left, bytes.length - at);
      this.image(bytes.subarray(at, at + take));
      p.left -= take;
      at += take;
    } else if (p.flg === 0) { // PACKED: p.left 128-bit register writes
      while (p.left > 0 && at + 16 <= bytes.length) {
        this.packed(bits(p.regs, 4 * (p.index % p.nreg), 4), view.getBigUint64(at, true), view.getBigUint64(at + 8, true));
        p.index += 1; p.left -= 1; at += 16;
      }
    } else { // REGLIST: p.left 64-bit register values, padded to a quadword
      while (p.left > 0 && at + 8 <= bytes.length) {
        const descriptor = bits(p.regs, 4 * (p.index % p.nreg), 4);
        if (descriptor !== 0xE && descriptor !== 0xF) this.write(descriptor, view.getBigUint64(at, true));
        p.index += 1; p.left -= 1; at += 8;
      }
      if (p.left === 0 && (p.words & 1)) at += 8;
    }
    if (p.left === 0) this.pending = null;
    return at;
  }

  packed(descriptor, low, high) {
    switch (descriptor) {
      case 0x0: this.write(0x00, low); break; // PRIM
      case 0x1: // RGBA: r, g, b, a in the low byte of each word
        this.rgbaq = { r: lo32(low) & 0xFF, g: Number(low >> 32n) & 0xFF, b: lo32(high) & 0xFF, a: Number(high >> 32n) & 0xFF, q: this.rgbaq.q };
        break;
      case 0x2: { // STQ
        const f = new Float32Array(new Uint32Array([lo32(low), Number(low >> 32n), lo32(high)]).buffer);
        this.st = { s: f[0], t: f[1] };
        this.rgbaq.q = f[2];
        break;
      }
      case 0x3: this.uv = { u: lo32(low) & 0x3FFF, v: Number(low >> 32n) & 0x3FFF }; break; // UV
      case 0x4: { // XYZF2 (packed: x, y, z << 4, f; ADC bit 111)
        const x = lo32(low) & 0xFFFF, y = Number(low >> 32n) & 0xFFFF;
        const z = (lo32(high) >>> 4) & 0xFFFFFF, f = Number(high >> 36n) & 0xFF, adc = Number(high >> 47n) & 1;
        this.fog = f;
        this.vertex(x, y, z, !adc);
        break;
      }
      case 0x5: { // XYZ2 (packed: x, y, z; ADC bit 111)
        const x = lo32(low) & 0xFFFF, y = Number(low >> 32n) & 0xFFFF, z = lo32(high);
        const adc = Number(high >> 47n) & 1;
        this.vertex(x, y, z, !adc);
        break;
      }
      case 0xA: this.fog = Number(high >> 36n) & 0xFF; break; // FOG
      case 0xD: { // XYZ3: never kicks
        const x = lo32(low) & 0xFFFF, y = Number(low >> 32n) & 0xFFFF;
        this.vertex(x, y, lo32(high), false);
        break;
      }
      case 0xC: { // XYZF3
        const x = lo32(low) & 0xFFFF, y = Number(low >> 32n) & 0xFFFF;
        this.vertex(x, y, (lo32(high) >>> 4) & 0xFFFFFF, false);
        break;
      }
      case 0xE: this.write(Number(high & 0xFFn), low); break; // A+D
      case 0xF: break; // NOP
      default: this.write(descriptor, low); // TEX0_1/2, CLAMP_1/2 as plain registers
    }
  }

  write(address, value) {
    switch (address) {
      case 0x00: this.prim = value; this.queue = []; break; // PRIM
      case 0x01: this.rgbaq = { r: bits(value, 0, 8), g: bits(value, 8, 8), b: bits(value, 16, 8), a: bits(value, 24, 8),
        q: new Float32Array(new Uint32Array([Number(value >> 32n)]).buffer)[0] }; break;
      case 0x02: { const f = new Float32Array(new Uint32Array([lo32(value), Number(value >> 32n)]).buffer); this.st = { s: f[0], t: f[1] }; break; }
      case 0x03: this.uv = { u: bits(value, 0, 14), v: bits(value, 16, 14) }; break;
      case 0x04: this.fog = bits(value, 56, 8); this.vertex(bits(value, 0, 16), bits(value, 16, 16), bits(value, 32, 24), true); break; // XYZF2
      case 0x05: this.vertex(bits(value, 0, 16), bits(value, 16, 16), Number(value >> 32n), true); break; // XYZ2
      case 0x0C: this.fog = bits(value, 56, 8); this.vertex(bits(value, 0, 16), bits(value, 16, 16), bits(value, 32, 24), false); break; // XYZF3
      case 0x0D: this.vertex(bits(value, 0, 16), bits(value, 16, 16), Number(value >> 32n), false); break; // XYZ3
      case 0x0A: this.fog = bits(value, 56, 8); break;
      case 0x0F: break; // NOP
      case 0x1B: this.reg.set(0x1B, value); break; // PRMODE
      case 0x3F: break; // TEXFLUSH
      case 0x53: this.reg.set(0x53, value); this.beginTransfer(); break; // TRXDIR
      case 0x60: this.note('SIGNAL'); break;
      case 0x61: break; // FINISH
      case 0x62: this.note('LABEL'); break;
      case 0x06: case 0x07: this.reg.set(address, value); this.loadClut(value); break; // TEX0_1/2
      default: this.reg.set(address, value);
    }
  }

  // ---- Local memory ----
  readPixel(psm, bp, bw, x, y) {
    const v = this.view;
    switch (psm) {
      case PSM.CT32: case PSM.Z32: return this.u32[(address32(bp, bw, x, y) & (MEMORY_BYTES - 1)) >> 2];
      case PSM.CT24: case PSM.Z24: return this.u32[(address32(bp, bw, x, y) & (MEMORY_BYTES - 1)) >> 2] & 0xFFFFFF;
      case PSM.CT16: case PSM.CT16S: case PSM.Z16: case PSM.Z16S: return v.getUint16(address16(bp, bw, x, y) & (MEMORY_BYTES - 1), true);
      case PSM.T8: return this.memory[address8(bp, bw, x, y) & (MEMORY_BYTES - 1)];
      case PSM.T4: { const n = address4(bp, bw, x, y); const byte = this.memory[(n >> 1) & (MEMORY_BYTES - 1)]; return n & 1 ? byte >> 4 : byte & 15; }
      case PSM.T8H: return this.memory[(address32(bp, bw, x, y) + 3) & (MEMORY_BYTES - 1)];
      case PSM.T4HL: return this.memory[(address32(bp, bw, x, y) + 3) & (MEMORY_BYTES - 1)] & 15;
      case PSM.T4HH: return this.memory[(address32(bp, bw, x, y) + 3) & (MEMORY_BYTES - 1)] >> 4;
      default: this.note(`read psm 0x${psm.toString(16)}`); return 0;
    }
  }

  writePixel(psm, bp, bw, x, y, value, mask = 0) {
    const v = this.view;
    switch (psm) {
      case PSM.CT32: case PSM.Z32: { const a = (address32(bp, bw, x, y) & (MEMORY_BYTES - 1)) >> 2; this.u32[a] = mask ? (this.u32[a] & mask) | (value & ~mask) : value; break; }
      case PSM.CT24: case PSM.Z24: { const a = address32(bp, bw, x, y) & (MEMORY_BYTES - 1); v.setUint32(a, (v.getUint32(a, true) & 0xFF000000) | (value & 0xFFFFFF), true); break; }
      case PSM.CT16: case PSM.CT16S: case PSM.Z16: case PSM.Z16S: v.setUint16(address16(bp, bw, x, y) & (MEMORY_BYTES - 1), value & 0xFFFF, true); break;
      case PSM.T8: this.memory[address8(bp, bw, x, y) & (MEMORY_BYTES - 1)] = value; break;
      case PSM.T4: { const n = address4(bp, bw, x, y); const a = (n >> 1) & (MEMORY_BYTES - 1); this.memory[a] = n & 1 ? (this.memory[a] & 0x0F) | ((value & 15) << 4) : (this.memory[a] & 0xF0) | (value & 15); break; }
      case PSM.T8H: this.memory[(address32(bp, bw, x, y) + 3) & (MEMORY_BYTES - 1)] = value; break;
      case PSM.T4HL: { const a = (address32(bp, bw, x, y) + 3) & (MEMORY_BYTES - 1); this.memory[a] = (this.memory[a] & 0xF0) | (value & 15); break; }
      case PSM.T4HH: { const a = (address32(bp, bw, x, y) + 3) & (MEMORY_BYTES - 1); this.memory[a] = (this.memory[a] & 0x0F) | ((value & 15) << 4); break; }
      default: this.note(`write psm 0x${psm.toString(16)}`);
    }
  }

  // ---- Host-to-local transfers (BITBLTBUF, TRXPOS, TRXREG, TRXDIR = 0, then IMAGE data) ----
  beginTransfer() {
    const dir = bits(this.reg.get(0x53) ?? 0n, 0, 2);
    if (dir !== 0) { this.note(`TRXDIR ${dir}`); this.transfer = null; return; }
    const bitblt = this.reg.get(0x50) ?? 0n, pos = this.reg.get(0x51) ?? 0n, size = this.reg.get(0x52) ?? 0n;
    this.transfer = { bp: bits(bitblt, 32, 14), bw: bits(bitblt, 48, 6), psm: bits(bitblt, 56, 6),
      x0: bits(pos, 32, 11), y0: bits(pos, 48, 11), w: bits(size, 0, 12), h: bits(size, 32, 12), x: 0, y: 0, pending: [] };
    this.stats.uploads += 1;
    if (this.trace) { (this.uploadLog ??= []).push({ dbp: this.transfer.bp.toString(16), dbw: this.transfer.bw, psm: this.transfer.psm, x: this.transfer.x0, y: this.transfer.y0, w: this.transfer.w, h: this.transfer.h }); if (this.uploadLog.length > 300) this.uploadLog.shift(); }
  }

  image(bytes) {
    const t = this.transfer;
    if (!t) { this.note('IMAGE without transfer'); return; }
    this.stats.uploadBytes += bytes.length;
    if (this.native) {
      const tr = this.nativeTransfer;
      tr.set([t.psm, t.bp, t.bw, t.x0, t.y0, t.w, t.h, t.x, t.y]);
      for (let at = 0; at < bytes.length; at += UPLOAD_CHUNK) {
        const part = bytes.subarray(at, Math.min(bytes.length, at + UPLOAD_CHUNK));
        this.nativeUpload.set(part);
        this.native.gs_upload(part.length);
      }
      t.x = tr[7]; t.y = tr[8];
      return;
    }
    const put = (value) => {
      if (t.y >= t.h) return;
      this.writePixel(t.psm, t.bp, t.bw, t.x0 + t.x, t.y0 + t.y, value);
      if (++t.x >= t.w) { t.x = 0; t.y += 1; }
    };
    const view = new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength);
    switch (t.psm) {
      case PSM.CT32: case PSM.Z32: for (let i = 0; i + 4 <= bytes.length; i += 4) put(view.getUint32(i, true)); break;
      case PSM.CT24: case PSM.Z24: for (let i = 0; i + 3 <= bytes.length; i += 3) put(bytes[i] | bytes[i + 1] << 8 | bytes[i + 2] << 16); break;
      case PSM.CT16: case PSM.CT16S: case PSM.Z16: case PSM.Z16S: for (let i = 0; i + 2 <= bytes.length; i += 2) put(view.getUint16(i, true)); break;
      case PSM.T8: case PSM.T8H: for (let i = 0; i < bytes.length; i++) put(bytes[i]); break;
      case PSM.T4: case PSM.T4HL: case PSM.T4HH: for (let i = 0; i < bytes.length; i++) { put(bytes[i] & 15); put(bytes[i] >> 4); } break;
      default: this.note(`upload psm 0x${t.psm.toString(16)}`);
    }
  }

  // An sceGsExecLoadImage upload: the GS image descriptor (sceGsLoadImage: GIF tag, BITBLTBUF,
  // TRXPOS, TRXREG, TRXDIR, then the IMAGE tag) is a GIF packet followed by the pixels.
  loadImage(descriptor, pixels) {
    this.gif(descriptor);
    this.image(pixels);
  }

  // ---- Palettes (CLD in TEX0: load the CLUT buffer from CBP) ----
  loadClut(tex0) {
    const cld = bits(tex0, 61, 3), cbp = bits(tex0, 37, 14), cpsm = bits(tex0, 51, 4), csa = bits(tex0, 56, 5), psm = bits(tex0, 20, 6);
    // CLD (GS User's Manual, TEX0): 0 keep; 1 load; 2 load, CBP0 = CBP; 3 load, CBP1 = CBP;
    // 4 load and set CBP0 only when CBP differs from CBP0; 5 the same with CBP1.
    if (cld === 0 || cld > 5) return;
    if (cld === 4) { if (this.cbp0 === cbp) return; this.cbp0 = cbp; }
    else if (cld === 5) { if (this.cbp1 === cbp) return; this.cbp1 = cbp; }
    else if (cld === 2) this.cbp0 = cbp;
    else if (cld === 3) this.cbp1 = cbp;
    const entries = psm === PSM.T8 || psm === PSM.T8H ? 256 : 16;
    if (this.native) { this.clut = this.nativeClut; this.clut.fill(0); } else this.clut = new Uint32Array(256);
    for (let i = 0; i < entries; i++) {
      // CSM1: 8-bit palettes are a 16 x 16 block with bits 3 and 4 of the index swapped; 4-bit
      // palettes a 8 x 2 block.
      const index = entries === 256 ? (i & 0xE7) | ((i & 0x08) << 1) | ((i & 0x10) >> 1) : i;
      const x = entries === 256 ? index & 15 : index & 7, y = entries === 256 ? index >> 4 : index >> 3;
      const raw = cpsm === PSM.CT32 ? this.readPixel(PSM.CT32, cbp, 1, x, y) : this.expand16(this.readPixel(PSM.CT16, cbp, 1, x, y));
      this.clut[entries === 256 ? i : (i + csa * 16) & 255] = raw;
    }
  }

  expand16(value) {
    const texa = this.reg.get(0x3B) ?? 0n;
    const ta0 = bits(texa, 0, 8), ta1 = bits(texa, 32, 8);
    const r = (value & 31) << 3, g = ((value >> 5) & 31) << 3, b = ((value >> 10) & 31) << 3;
    return (r | g << 8 | b << 16 | ((value & 0x8000 ? ta1 : ta0) << 24)) >>> 0;
  }

  // A texture as the current TEX0 and CLAMP describe it, decoded once per primitive.
  textureState(tex0, clamp) {
    const psm = bits(tex0, 20, 6);
    return {
      tex0, tbp: bits(tex0, 0, 14), tbw: bits(tex0, 14, 6), psm,
      tw: 1 << bits(tex0, 26, 4), th: 1 << bits(tex0, 30, 4), tcc: bits(tex0, 34, 1), tfx: bits(tex0, 35, 2),
      indexed: psm === PSM.T8 || psm === PSM.T4 || psm === PSM.T8H || psm === PSM.T4HL || psm === PSM.T4HH,
      wms: bits(clamp, 0, 2), wmt: bits(clamp, 2, 2), minu: bits(clamp, 4, 10), maxu: bits(clamp, 14, 10),
      minv: bits(clamp, 24, 10), maxv: bits(clamp, 34, 10), ta0: bits(this.reg.get(0x3B) ?? 0n, 0, 8),
    };
  }

  // One texel as packed RGBA (alpha 0x80 when TCC is 0) into this.texelColor.
  sample(t, u, v) {
    switch (t.wms) {
      case 0: u &= t.tw - 1; break;
      case 1: u = u < 0 ? 0 : u >= t.tw ? t.tw - 1 : u; break;
      case 2: u = u < t.minu ? t.minu : u > t.maxu ? t.maxu : u; break;
      default: u = (u & t.minu) | t.maxu;
    }
    switch (t.wmt) {
      case 0: v &= t.th - 1; break;
      case 1: v = v < 0 ? 0 : v >= t.th ? t.th - 1 : v; break;
      case 2: v = v < t.minv ? t.minv : v > t.maxv ? t.maxv : v; break;
      default: v = (v & t.minv) | t.maxv;
    }
    const raw = this.readPixel(t.psm, t.tbp, t.tbw, u, v);
    let color;
    if (t.indexed) color = this.clut ? this.clut[raw] : 0;
    else if (t.psm === PSM.CT16 || t.psm === PSM.CT16S) color = this.expand16(raw);
    else if (t.psm === PSM.CT24) color = (raw | (t.ta0 << 24)) >>> 0;
    else color = raw;
    return t.tcc ? color >>> 0 : ((color & 0xFFFFFF) | 0x80000000) >>> 0;
  }

  // Bilinear: the four texels around (u - 0.5, v - 0.5), weighted by the fractions (the GS keeps
  // four fraction bits), each channel separately.
  sampleBilinear(t, u, v) {
    const us = u - 0.5, vs = v - 0.5;
    const u0 = Math.floor(us), v0 = Math.floor(vs);
    const fu = Math.floor((us - u0) * 16) / 16, fv = Math.floor((vs - v0) * 16) / 16;
    const c00 = this.sample(t, u0, v0), c10 = this.sample(t, u0 + 1, v0), c01 = this.sample(t, u0, v0 + 1), c11 = this.sample(t, u0 + 1, v0 + 1);
    let out = 0;
    for (let shift = 0; shift < 32; shift += 8) {
      const a = (c00 >>> shift) & 255, b = (c10 >>> shift) & 255, cc = (c01 >>> shift) & 255, d = (c11 >>> shift) & 255;
      const top = a + (b - a) * fu, bottom = cc + (d - cc) * fu;
      out |= (Math.floor(top + (bottom - top) * fv) & 255) << shift;
    }
    return out >>> 0;
  }

  texel(tex0, u, v) { // single lookups (diagnosis tools)
    const color = this.sample(this.textureState(tex0, this.reg.get(this.context ? 0x09 : 0x08) ?? 0n), u, v);
    return { r: color & 255, g: (color >>> 8) & 255, b: (color >>> 16) & 255, a: color >>> 24, hasAlpha: 1 };
  }

  // ---- Vertices and primitives ----
  vertex(x, y, z, kick) {
    const context = bits(this.prim, 9, 1);
    this.context = context;
    const offset = this.reg.get(context ? 0x19 : 0x18) ?? 0n;
    const v = { x: (x - bits(offset, 0, 16)) / 16, y: (y - bits(offset, 32, 16)) / 16, z,
      r: this.rgbaq.r, g: this.rgbaq.g, b: this.rgbaq.b, a: this.rgbaq.a, q: this.rgbaq.q,
      s: this.st.s, t: this.st.t, u: this.uv.u / 16, vv: this.uv.v / 16, f: this.fog };
    this.queue.push(v);
    const type = bits(this.prim, 0, 3);
    const need = [1, 2, 2, 3, 3, 3, 2, 0][type];
    if (need === 0) { this.queue = []; return; }
    if (this.queue.length < need) return;
    if (kick) this.draw(type);
    // Keep what the next vertex shares (strips and fans), drop the rest.
    if (type === 2) this.queue = [this.queue[this.queue.length - 1]];
    else if (type === 4) this.queue = this.queue.slice(-2);
    else if (type === 5) this.queue = [this.queue[0], this.queue[this.queue.length - 1]];
    else this.queue = [];
  }

  // The drawing state of the current context, decoded once per primitive.
  state() {
    const c = this.context;
    // PRMODECONT AC = 1 (the default): attributes from PRIM; 0: from PRMODE.
    const ac = this.reg.has(0x1A) ? bits(this.reg.get(0x1A), 0, 1) : 1;
    const prim = ac ? this.prim : (this.reg.get(0x1B) ?? 0n);
    const tex0 = this.reg.get(c ? 0x07 : 0x06) ?? 0n, frame = this.reg.get(c ? 0x4D : 0x4C) ?? 0n;
    const zbuf = this.reg.get(c ? 0x4F : 0x4E) ?? 0n, test = this.reg.get(c ? 0x48 : 0x47) ?? 0n;
    const alpha = this.reg.get(c ? 0x43 : 0x42) ?? 0n, scissor = this.reg.get(c ? 0x41 : 0x40) ?? 0n;
    const s = {
      tme: bits(prim, 4, 1), fst: bits(prim, 8, 1), abe: bits(prim, 6, 1), iip: bits(prim, 3, 1),
      tex0, scissor, test,
      scx0: bits(scissor, 0, 11), scx1: bits(scissor, 16, 11), scy0: bits(scissor, 32, 11), scy1: bits(scissor, 48, 11),
      ate: bits(test, 0, 1), atst: bits(test, 1, 3), aref: bits(test, 4, 8), afail: bits(test, 12, 2),
      zte: bits(test, 16, 1), ztst: bits(test, 17, 2),
      zbp: bits(zbuf, 0, 9) * 32, zpsm: bits(zbuf, 24, 4) | 0x30, zmsk: bits(zbuf, 32, 1),
      fbp: bits(frame, 0, 9) * 32, fbw: bits(frame, 16, 6), fpsm: bits(frame, 24, 6), fbmsk: bits(frame, 32, 32) >>> 0,
      asel: bits(alpha, 0, 2), bsel: bits(alpha, 2, 2), csel: bits(alpha, 4, 2), dsel: bits(alpha, 6, 2), fix: bits(alpha, 32, 8),
      fba: bits(this.reg.get(c ? 0x4B : 0x4A) ?? 0n, 0, 1) ? 0x80 : 0, pabe: bits(this.reg.get(0x49) ?? 0n, 0, 1),
      texture: null,
    };
    s.fb16 = s.fpsm === PSM.CT16 || s.fpsm === PSM.CT16S;
    if (s.tme) {
      s.texture = this.textureState(tex0, this.reg.get(c ? 0x09 : 0x08) ?? 0n);
      // TEX1 (GS User's Manual): LCM bit 0, L bits 19-20, K bits 32-43 (signed, 1/16 steps), MMAG
      // bit 5 (1 = bilinear when LOD <= 0), MMIN bits 6-8 (1, 4, 5 = bilinear when LOD > 0; the
      // mipmap levels themselves are not sampled here: level 0 is).
      const tex1 = this.reg.get(c ? 0x15 : 0x14) ?? 0n;
      const k = bits(tex1, 32, 12);
      s.texture.lcm = bits(tex1, 0, 1); s.texture.lodL = bits(tex1, 19, 2); s.texture.lodK = (k & 0x800 ? k - 0x1000 : k) / 16;
      s.texture.mmag = bits(tex1, 5, 1); const mmin = bits(tex1, 6, 3); s.texture.mminLinear = mmin === 1 || mmin === 4 || mmin === 5 ? 1 : 0;
      const key = `psm 0x${s.texture.psm.toString(16)} tfx ${s.texture.tfx}`;
      this.stats.formats.set(key, (this.stats.formats.get(key) ?? 0) + 1);
    }
    return s;
  }

  draw(type) {
    this.stats.primitives += 1;
    const q = this.queue;
    const s = this.state();
    if (this.trace) { // last primitives, for diagnosis
      const vs = q.slice(type === 6 ? -2 : type >= 3 ? -3 : -2);
      this.trace.push({ type, box: [Math.min(...vs.map((v) => v.x)), Math.min(...vs.map((v) => v.y)), Math.max(...vs.map((v) => v.x)), Math.max(...vs.map((v) => v.y))].map((n) => Math.round(n)),
        tme: s.tme, psm: bits(s.tex0, 20, 6), tex0: '0x' + s.tex0.toString(16), clamp: '0x' + (this.reg.get(this.context ? 0x09 : 0x08) ?? 0n).toString(16),
        uv: vs.map((v) => [v.u, v.vv]), scissor: [s.scx0, s.scx1, s.scy0, s.scy1], rgba: [vs[0].r, vs[0].g, vs[0].b, vs[0].a], abe: s.abe, test: Number(s.test & 0xFFFFn),
        frame: [s.fbp, s.fbw, s.fpsm], fbmsk: (s.fbmsk >>> 0).toString(16), zte: s.zte, ztst: s.ztst, zbp: s.zbp, blend: [s.asel, s.bsel, s.csel, s.dsel, s.fix], tfx: s.texture?.tfx, tcc: s.texture?.tcc, fba: s.fba, pabe: s.pabe });
      if (this.trace.length > 400) this.trace.shift();
    }
    if (this.trace && s.texture && this.captureTbp === s.texture.tbp && s.texture.tw === 256 && !this.captureOnce) { if (process.env.GS_CAPTURE_FIRST) this.captureOnce = true; // diagnosis: the texture as drawn
      const t = s.texture, out = new Uint8Array(t.tw * t.th * 4);
      for (let y = 0; y < t.th; y++) for (let x = 0; x < t.tw; x++) { const c = this.sample({ ...t, wms: 0, wmt: 0 }, x, y); out.set([c & 255, (c >>> 8) & 255, (c >>> 16) & 255, c >>> 24], (y * t.tw + x) * 4); }
      this.captured = { width: t.tw, height: t.th, rgba: out, clut: Array.from(this.clut ?? []).slice(0, 16).map((c) => c.toString(16)) };
    }
    // Optional (the page turns it on): skip primitives that sample the frame buffer they draw into,
    // i.e. the game's full-screen softening pass at the end of each frame (16 sprites re-reading the
    // frame with bilinear filtering). The page draws the 3D world at full resolution and wants a
    // sharp picture; this pass is the GS's single largest job. The Node reference keeps it.
    if (this.skipFrameFeedback && s.texture && s.texture.tbp === s.fbp) { this.stats.skippedFeedback = (this.stats.skippedFeedback ?? 0) + 1; return; }
    // Optional (the page sets it during gameplay): skip the sprites that copy the draw buffer
    // (512x416) to the display buffer, stretching it to 448 lines with bilinear filtering. The page
    // shows the draw buffer itself, scaled once by the browser, under its full-resolution world.
    if (this.displaySourceTbp !== undefined && type === 6 && s.texture && s.texture.tbp === this.displaySourceTbp && s.fbp !== s.texture.tbp) { this.stats.skippedDisplayCopy = (this.stats.skippedDisplayCopy ?? 0) + 1; return; }
    if (this.probePoint && this.afterSky) { // diagnosis: primitives whose box covers the probe point
      const n = type === 6 ? 2 : type >= 3 ? 3 : type === 0 ? 1 : 2, vs = q.slice(-n);
      const xs = vs.map((v) => v.x), ys = vs.map((v) => v.y), [px, py] = this.probePoint;
      if (Math.min(...xs) <= px && px <= Math.max(...xs) && Math.min(...ys) <= py && py <= Math.max(...ys)) {
        const t = s.texture;
        const key = `type ${type} tex ${t ? `${t.psm.toString(16)}@${t.tbp.toString(16)}` : 'none'} zte ${s.zte} ztst ${s.ztst} abe ${s.abe} fst ${s.fst} z ${Math.round(vs[0].z)}`;
        this.probeHits.set(key, (this.probeHits.get(key) ?? 0) + 1);
      }
    }
    if (this.native) { this.drawNative(type, q, s); return; }
    if (type === 6) this.sprite(q[q.length - 2], q[q.length - 1], s);
    else if (type >= 3) this.triangle(q[q.length - 3], q[q.length - 2], q[q.length - 1], s);
    else if (type === 0) { this.setPoint(q[0], s); this.pixelAt(Math.floor(q[0].x), Math.floor(q[0].y), this.p, s); }
    else this.line(q[q.length - 2], q[q.length - 1], s);
  }

  drawNative(type, q, s) {
    const st = this.nativeState;
    STATE_FIELDS.forEach((name, i) => { st[i] = s[name] | 0; });
    const t = s.texture;
    TEXTURE_FIELDS.forEach((name, i) => { st[STATE_FIELDS.length + i] = t ? (t[name] | 0) : 0; });
    const texa = this.reg.get(0x3B) ?? 0n;
    st[STATE_FIELDS.length + TEXTURE_FIELDS.length] = this.clut ? 1 : 0;
    st[STATE_FIELDS.length + TEXTURE_FIELDS.length + 1] = bits(texa, 0, 8);
    st[STATE_FIELDS.length + TEXTURE_FIELDS.length + 2] = bits(texa, 32, 8);
    this.nativeLodK[0] = t ? t.lodK : 0;
    if (this.clut && this.clut !== this.nativeClut) this.nativeClut.set(this.clut);
    const count = type === 6 ? 2 : type >= 3 ? 3 : type === 0 ? 1 : 2;
    const vs = this.nativeVertices;
    for (let i = 0; i < count; i++) {
      const v = q[q.length - count + i], o = i * 12;
      vs[o] = v.x; vs[o + 1] = v.y; vs[o + 2] = v.z; vs[o + 3] = v.r; vs[o + 4] = v.g; vs[o + 5] = v.b; vs[o + 6] = v.a;
      vs[o + 7] = v.q; vs[o + 8] = v.s; vs[o + 9] = v.t; vs[o + 10] = v.u; vs[o + 11] = v.vv;
    }
    const before = this.native.gs_pixel_count();
    const started = this.costs ? performance.now() : 0;
    this.native.gs_draw(type);
    const drawn = (this.native.gs_pixel_count() - before) >>> 0;
    this.stats.pixels += drawn;
    if (this.costs) { // diagnosis: time and pixels per kind of primitive
      const t = s.texture;
      const key = `type ${type} tex ${t ? `${t.psm.toString(16)}@${t.tbp.toString(16)} ${t.mmag ? 'bilinear' : 'nearest'}` : 'none'} -> fb ${s.fbp.toString(16)} abe ${s.abe} zte ${s.zte} ${this.phase}`;
      const c = this.costs.get(key) ?? { n: 0, px: 0, ms: 0 };
      c.n += 1; c.px += drawn; c.ms += performance.now() - started;
      this.costs.set(key, c);
    }
  }

  // The browser-drawn world's depth (world-renderer.mjs gsDepth: GS depth in rgb, a = covered, rows
  // bottom-up) into the context-1 Z buffer (ZBUF_1), so the game's later 3D drawing tests against it.
  blitZ(packed, width, height) {
    if (!this.native || packed.length > this.nativeUpload.length) return false;
    const frame = this.reg.get(0x4C) ?? 0n, zbuf = this.reg.get(0x4E) ?? 0n;
    this.nativeUpload.set(packed);
    this.native.gs_blit_z(bits(zbuf, 0, 9) * 32, bits(frame, 16, 6), bits(zbuf, 24, 4) | 0x30, width, height);
    return true;
  }

  // A picture drawn outside the GS (world-renderer.mjs: RGBA, rows bottom-up) into the current
  // context-1 frame buffer, where its alpha is nonzero. Needs the WebAssembly rasterizer.
  blitWorld(rgba, width, height) {
    if (!this.native || rgba.length > this.nativeUpload.length) return false;
    const frame = this.reg.get(0x4C) ?? 0n;
    this.nativeUpload.set(rgba);
    this.native.gs_blit_rgba(bits(frame, 0, 9) * 32, bits(frame, 16, 6), bits(frame, 24, 6), width, height);
    return true;
  }

  setPoint(v, s) {
    const p = this.p;
    p.r = v.r; p.g = v.g; p.b = v.b; p.a = v.a; p.z = v.z;
    if (s.fst) { p.u = v.u; p.vv = v.vv; } else { p.s = v.s; p.t = v.t; p.q = v.q; }
  }

  sprite(a, b, s) {
    const p = this.p;
    p.r = b.r; p.g = b.g; p.b = b.b; p.a = b.a; p.z = b.z; p.q = b.q;
    const x0 = Math.max(Math.ceil(Math.min(a.x, b.x)), s.scx0), x1 = Math.min(Math.max(a.x, b.x), s.scx1 + 1);
    const y0 = Math.max(Math.ceil(Math.min(a.y, b.y)), s.scy0), y1 = Math.min(Math.max(a.y, b.y), s.scy1 + 1);
    const dx = (b.x - a.x) || 1, dy = (b.y - a.y) || 1;
    for (let y = y0; y < y1; y++) {
      const fy = (y - a.y) / dy;
      if (s.fst) p.vv = a.vv + (b.vv - a.vv) * fy; else p.t = a.t + (b.t - a.t) * fy;
      for (let x = x0; x < x1; x++) {
        const fx = (x - a.x) / dx;
        if (s.fst) p.u = a.u + (b.u - a.u) * fx; else p.s = a.s + (b.s - a.s) * fx;
        this.pixelAt(x, y, p, s);
      }
    }
  }

  line(a, b, s) {
    const steps = Math.max(Math.abs(b.x - a.x), Math.abs(b.y - a.y), 1);
    for (let i = 0; i <= steps; i++) {
      const f = i / steps;
      this.mix(a, b, b, 1 - f, f, 0, s);
      this.pixelAt(Math.floor(a.x + (b.x - a.x) * f), Math.floor(a.y + (b.y - a.y) * f), this.p, s);
    }
  }

  mix(a, b, c, wa, wb, wc, s) {
    const p = this.p;
    p.z = a.z * wa + b.z * wb + c.z * wc;
    if (s.iip) { p.r = a.r * wa + b.r * wb + c.r * wc; p.g = a.g * wa + b.g * wb + c.g * wc; p.b = a.b * wa + b.b * wb + c.b * wc; p.a = a.a * wa + b.a * wb + c.a * wc; }
    else { p.r = c.r; p.g = c.g; p.b = c.b; p.a = c.a; }
    if (s.fst) { p.u = a.u * wa + b.u * wb + c.u * wc; p.vv = a.vv * wa + b.vv * wb + c.vv * wc; }
    else { p.s = a.s * wa + b.s * wb + c.s * wc; p.t = a.t * wa + b.t * wb + c.t * wc; p.q = a.q * wa + b.q * wb + c.q * wc; }
  }

  triangle(a, b, c, s) {
    const area = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
    if (area === 0) return;
    const minX = Math.max(Math.floor(Math.min(a.x, b.x, c.x)), s.scx0), maxX = Math.min(Math.ceil(Math.max(a.x, b.x, c.x)), s.scx1);
    const minY = Math.max(Math.floor(Math.min(a.y, b.y, c.y)), s.scy0), maxY = Math.min(Math.ceil(Math.max(a.y, b.y, c.y)), s.scy1);
    for (let y = minY; y <= maxY; y++) {
      const py = y + 0.5;
      for (let x = minX; x <= maxX; x++) {
        const px = x + 0.5;
        const w0 = ((b.x - px) * (c.y - py) - (c.x - px) * (b.y - py)) / area;
        if (w0 < 0) continue;
        const w1 = ((c.x - px) * (a.y - py) - (a.x - px) * (c.y - py)) / area;
        const w2 = 1 - w0 - w1;
        if (w1 < 0 || w2 < 0) continue;
        this.mix(a, b, c, w0, w1, w2, s);
        this.pixelAt(x, y, this.p, s);
      }
    }
  }

  pixelAt(x, y, p, s) {
    if (x < s.scx0 || x > s.scx1 || y < s.scy0 || y > s.scy1) return;
    let r = p.r | 0, g = p.g | 0, b = p.b | 0, a = p.a | 0;
    const t = s.texture;
    if (t) {
      let uf, vf, q = 1;
      if (s.fst) { uf = p.u; vf = p.vv; }
      else { q = p.q || 1; uf = (p.s / q) * t.tw; vf = (p.t / q) * t.th; }
      const lod = t.lcm ? t.lodK : (s.fst ? 0 : -Math.log2(Math.abs(q)) * (1 << t.lodL)) + t.lodK;
      const c = (lod <= 0 ? t.mmag : t.mminLinear) ? this.sampleBilinear(t, uf, vf) : this.sample(t, Math.floor(uf), Math.floor(vf));
      const tr = c & 255, tg = (c >>> 8) & 255, tb = (c >>> 16) & 255, ta = c >>> 24;
      switch (t.tfx) {
        case 0: r = (tr * r) >> 7; g = (tg * g) >> 7; b = (tb * b) >> 7; a = t.tcc ? (ta * a) >> 7 : a; break; // MODULATE
        case 1: r = tr; g = tg; b = tb; a = t.tcc ? ta : a; break; // DECAL
        case 2: r = ((tr * r) >> 7) + a; g = ((tg * g) >> 7) + a; b = ((tb * b) >> 7) + a; a = t.tcc ? ta + a : a; break; // HIGHLIGHT
        default: r = ((tr * r) >> 7) + a; g = ((tg * g) >> 7) + a; b = ((tb * b) >> 7) + a; a = t.tcc ? ta : a; // HIGHLIGHT2
      }
      if (r > 255) r = 255; if (g > 255) g = 255; if (b > 255) b = 255; if (a > 255) a = 255;
    }
    let writeColor = true, writeZ = true;
    if (s.ate) { // alpha test (ATST, AREF, AFAIL)
      const aref = s.aref;
      let pass;
      switch (s.atst) { case 0: pass = false; break; case 1: pass = true; break; case 2: pass = a < aref; break; case 3: pass = a <= aref; break;
        case 4: pass = a === aref; break; case 5: pass = a >= aref; break; case 6: pass = a > aref; break; default: pass = a !== aref; }
      if (!pass) {
        if (s.afail === 0) return;
        if (s.afail === 2) writeColor = false; else writeZ = false;
      }
    }
    const z = p.z <= 0 ? 0 : p.z >= 0xFFFFFFFF ? 0xFFFFFFFF : Math.floor(p.z);
    if (s.zte) { // Z test against the Z buffer
      if (s.ztst === 0) return;
      if (s.ztst >= 2) {
        const current = this.readPixel(s.zpsm, s.zbp, s.fbw, x, y);
        if (s.ztst === 2 ? z < current : z <= current) return;
      }
    }
    if (writeColor) {
      if (s.abe && !(s.pabe && a < 0x80)) { // ((A - B) * C >> 7) + D
        const dst = this.readPixel(s.fb16 ? s.fpsm : PSM.CT32, s.fbp, s.fbw, x, y);
        const d = s.fb16 ? this.expand16(dst) : dst;
        const dr = d & 255, dg = (d >>> 8) & 255, db = (d >>> 16) & 255;
        const cv = s.csel === 0 ? a : s.csel === 1 ? d >>> 24 : s.fix;
        const pa = (sel, src, dst2) => (sel === 0 ? src : sel === 1 ? dst2 : 0);
        let nr = (((pa(s.asel, r, dr) - pa(s.bsel, r, dr)) * cv) >> 7) + pa(s.dsel, r, dr);
        let ng = (((pa(s.asel, g, dg) - pa(s.bsel, g, dg)) * cv) >> 7) + pa(s.dsel, g, dg);
        let nb = (((pa(s.asel, b, db) - pa(s.bsel, b, db)) * cv) >> 7) + pa(s.dsel, b, db);
        r = nr < 0 ? 0 : nr > 255 ? 255 : nr; g = ng < 0 ? 0 : ng > 255 ? 255 : ng; b = nb < 0 ? 0 : nb > 255 ? 255 : nb;
      }
      if (s.fb16) this.writePixel(s.fpsm, s.fbp, s.fbw, x, y, (r >> 3) | (g >> 3) << 5 | (b >> 3) << 10 | (a & 0x80 ? 0x8000 : 0));
      else this.writePixel(s.fpsm, s.fbp, s.fbw, x, y, (r | g << 8 | b << 16 | ((a | s.fba) & 0xFF) << 24) >>> 0, s.fbmsk);
      this.stats.pixels += 1;
    }
    if (writeZ && !s.zmsk && s.zte) this.writePixel(s.zpsm, s.zbp, s.fbw, x, y, z);
  }

  // The context-1 frame buffer being drawn (FRAME_1), as framebuffer() reads a displayed one.
  frameBufferRgba(width, height) {
    const frame = this.reg.get(0x4C) ?? 0n;
    const asDispfb = BigInt(bits(frame, 0, 9)) | (BigInt(bits(frame, 16, 6)) << 9n) | (BigInt(bits(frame, 24, 5)) << 15n);
    return { fbp: bits(frame, 0, 9), dispfb: asDispfb, rgba: this.framebuffer(asDispfb, width, height) };
  }

  // The displayed picture: the frame buffer DISPFB names (FBP in 2048-word pages, FBW, PSM,
  // DBX/DBY), width x height pixels, as RGBA bytes with alpha forced opaque.
  framebuffer(dispfb, width, height) {
    const fbp = bits(dispfb, 0, 9) * 32, fbw = bits(dispfb, 9, 6), psm = bits(dispfb, 15, 5);
    const dbx = bits(dispfb, 32, 11), dby = bits(dispfb, 43, 11);
    if (this.native) {
      const texa = this.reg.get(0x3B) ?? 0n;
      this.nativeState[STATE_FIELDS.length + TEXTURE_FIELDS.length + 1] = bits(texa, 0, 8);
      this.nativeState[STATE_FIELDS.length + TEXTURE_FIELDS.length + 2] = bits(texa, 32, 8);
      this.native.gs_framebuffer(fbp, fbw, psm, dbx, dby, width, height);
      return new Uint8Array(this.native.memory.buffer, this.native.gs_out(), width * height * 4).slice();
    }
    const out = new Uint8Array(width * height * 4);
    for (let y = 0; y < height; y++) {
      for (let x = 0; x < width; x++) {
        const raw = this.readPixel(psm, fbp, fbw, x + dbx, y + dby);
        const c = psm === PSM.CT16 || psm === PSM.CT16S ? this.expand16(raw) : raw;
        const o = (y * width + x) * 4;
        out[o] = c & 255; out[o + 1] = (c >>> 8) & 255; out[o + 2] = (c >>> 16) & 255; out[o + 3] = 255;
      }
    }
    return out;
  }
}
