/* The GS rasterizer of browser/gs-renderer.mjs in C, compiled to WebAssembly for speed. The
   JavaScript renderer is the reference: this file follows it line for line (doubles where it uses
   JavaScript numbers, JavaScript's ToInt32 where it uses `| 0`), so both draw identical frames
   (checked by comparing frame dumps, see docs/build/native-browser-pipeline.md).

   What lives here: the 4 MiB of GS local memory with its swizzled pixel layouts, host-to-local
   uploads, texture sampling (nearest and bilinear, CLUT, TEXA), the sprite / line / triangle /
   point rasterizers with the per-pixel pipeline (texture function, alpha test, Z test, alpha
   blending, frame-buffer mask, 16-bit frame buffers), and the display read-out. The GIF tag
   parsing and register decoding stay in JavaScript, which fills `gs_state` and `gs_vertices`
   before each draw. */
#include <math.h>
#include <stdint.h>

#define MEMORY_BYTES (4 * 1024 * 1024)
#define MASK (MEMORY_BYTES - 1)

enum { CT32 = 0x00, CT24 = 0x01, CT16 = 0x02, CT16S = 0x0A, T8 = 0x13, T4 = 0x14, T8H = 0x1B, T4HL = 0x24, T4HH = 0x2C,
       Z32 = 0x30, Z24 = 0x31, Z16 = 0x32, Z16S = 0x3A };

static uint8_t memory[MEMORY_BYTES] __attribute__((aligned(16)));
static uint32_t clut[256];
static uint8_t out_rgba[1024 * 1024 * 4];
static uint8_t upload_buffer[1 << 20];

/* Swizzle tables (GS User's Manual, ch. 8), folded per page: one offset per pixel of a page.
   gs-renderer.mjs builds them from its block and column tables and copies them in (gs_table), so
   the JavaScript and C renderers use the same tables. */
static int32_t SWZ32[32 * 64], SWZ16[64 * 64], SWZ8[64 * 128], SWZ4[128 * 128];

int32_t *gs_table(int32_t which) {
    return which == 0 ? SWZ32 : which == 1 ? SWZ16 : which == 2 ? SWZ8 : SWZ4;
}

static inline uint32_t address32(uint32_t bp, uint32_t bw, int32_t x, int32_t y) {
    return (uint32_t)((int32_t)(bp * 64 + ((y >> 5) * (int32_t)bw + (x >> 6)) * 2048 + SWZ32[((y & 31) << 6) | (x & 63)]) * 4);
}
static inline uint32_t address16(uint32_t bp, uint32_t bw, int32_t x, int32_t y) {
    return (uint32_t)((int32_t)(bp * 128 + ((y >> 6) * (int32_t)bw + (x >> 6)) * 4096 + SWZ16[((y & 63) << 6) | (x & 63)]) * 2);
}
static inline uint32_t address8(uint32_t bp, uint32_t bw, int32_t x, int32_t y) {
    return (uint32_t)((int32_t)(bp * 256) + ((y >> 6) * (int32_t)(bw >> 1) + (x >> 7)) * 8192 + SWZ8[((y & 63) << 7) | (x & 127)]);
}
static inline uint32_t address4(uint32_t bp, uint32_t bw, int32_t x, int32_t y) {
    return (uint32_t)((int32_t)(bp * 512) + ((y >> 7) * (int32_t)(bw >> 1) + (x >> 7)) * 16384 + SWZ4[((y & 127) << 7) | (x & 127)]);
}

static inline uint32_t rd32(uint32_t a) { return *(uint32_t *)(memory + (a & MASK & ~3u)); }
static inline uint16_t rd16(uint32_t a) { return *(uint16_t *)(memory + (a & MASK & ~1u)); }

static inline __attribute__((always_inline)) uint32_t read_pixel(int32_t psm, uint32_t bp, uint32_t bw, int32_t x, int32_t y) {
    switch (psm) {
    case CT32: case Z32: return rd32(address32(bp, bw, x, y));
    case CT24: case Z24: return rd32(address32(bp, bw, x, y)) & 0xFFFFFF;
    case CT16: case CT16S: case Z16: case Z16S: return rd16(address16(bp, bw, x, y));
    case T8: return memory[address8(bp, bw, x, y) & MASK];
    case T4: { const uint32_t n = address4(bp, bw, x, y); const uint8_t byte = memory[(n >> 1) & MASK]; return n & 1 ? byte >> 4 : byte & 15; }
    case T8H: return memory[(address32(bp, bw, x, y) + 3) & MASK];
    case T4HL: return memory[(address32(bp, bw, x, y) + 3) & MASK] & 15;
    case T4HH: return memory[(address32(bp, bw, x, y) + 3) & MASK] >> 4;
    default: return 0;
    }
}

static inline __attribute__((always_inline)) void write_pixel(int32_t psm, uint32_t bp, uint32_t bw, int32_t x, int32_t y, uint32_t value, uint32_t mask) {
    switch (psm) {
    case CT32: case Z32: { uint32_t *p = (uint32_t *)(memory + (address32(bp, bw, x, y) & MASK & ~3u)); *p = mask ? (*p & mask) | (value & ~mask) : value; break; }
    case CT24: case Z24: { uint32_t *p = (uint32_t *)(memory + (address32(bp, bw, x, y) & MASK & ~3u)); *p = (*p & 0xFF000000u) | (value & 0xFFFFFF); break; }
    case CT16: case CT16S: case Z16: case Z16S: *(uint16_t *)(memory + (address16(bp, bw, x, y) & MASK & ~1u)) = (uint16_t)value; break;
    case T8: memory[address8(bp, bw, x, y) & MASK] = (uint8_t)value; break;
    case T4: { const uint32_t n = address4(bp, bw, x, y); const uint32_t a = (n >> 1) & MASK; memory[a] = n & 1 ? (memory[a] & 0x0F) | ((value & 15) << 4) : (memory[a] & 0xF0) | (value & 15); break; }
    case T8H: memory[(address32(bp, bw, x, y) + 3) & MASK] = (uint8_t)value; break;
    case T4HL: { const uint32_t a = (address32(bp, bw, x, y) + 3) & MASK; memory[a] = (memory[a] & 0xF0) | (value & 15); break; }
    case T4HH: { const uint32_t a = (address32(bp, bw, x, y) + 3) & MASK; memory[a] = (memory[a] & 0x0F) | ((value & 15) << 4); break; }
    default: break;
    }
}

uint32_t gs_read_pixel(int32_t psm, uint32_t bp, uint32_t bw, int32_t x, int32_t y) { return read_pixel(psm, bp, bw, x, y); }
void gs_write_pixel(int32_t psm, uint32_t bp, uint32_t bw, int32_t x, int32_t y, uint32_t value, uint32_t mask) { write_pixel(psm, bp, bw, x, y, value, mask); }

/* ---- Uploads: the pixels of an IMAGE transfer, in the transfer's format. ---- */
/* transfer: psm, bp, bw, x0, y0, w, h, x, y (x, y updated). */
int32_t gs_transfer[9];
void gs_upload(uint32_t bytes) {
    int32_t *t = gs_transfer;
    const uint8_t *b = upload_buffer;
#define PUT(value) do { if (t[8] < t[6]) { write_pixel(t[0], (uint32_t)t[1], (uint32_t)t[2], t[3] + t[7], t[4] + t[8], (value), 0); if (++t[7] >= t[5]) { t[7] = 0; t[8] += 1; } } } while (0)
    switch (t[0]) {
    case CT32: case Z32: for (uint32_t i = 0; i + 4 <= bytes; i += 4) PUT(*(const uint32_t *)(b + i)); break;
    case CT24: case Z24: for (uint32_t i = 0; i + 3 <= bytes; i += 3) PUT((uint32_t)b[i] | (uint32_t)b[i + 1] << 8 | (uint32_t)b[i + 2] << 16); break;
    case CT16: case CT16S: case Z16: case Z16S: for (uint32_t i = 0; i + 2 <= bytes; i += 2) PUT(*(const uint16_t *)(b + i)); break;
    case T8: case T8H: for (uint32_t i = 0; i < bytes; i++) PUT(b[i]); break;
    case T4: case T4HL: case T4HH: for (uint32_t i = 0; i < bytes; i++) { PUT(b[i] & 15u); PUT(b[i] >> 4); } break;
    default: break;
    }
#undef PUT
}

/* ---- The drawing state, filled by JavaScript per primitive (gs-renderer.mjs nativeState). ---- */
enum {
    S_TME, S_FST, S_ABE, S_IIP, S_SCX0, S_SCX1, S_SCY0, S_SCY1, S_ATE, S_ATST, S_AREF, S_AFAIL, S_ZTE, S_ZTST,
    S_ZBP, S_ZPSM, S_ZMSK, S_FBP, S_FBW, S_FPSM, S_FBMSK, S_ASEL, S_BSEL, S_CSEL, S_DSEL, S_FIX, S_FBA, S_PABE, S_FB16,
    S_TBP, S_TBW, S_PSM, S_TW, S_TH, S_TCC, S_TFX, S_INDEXED, S_WMS, S_WMT, S_MINU, S_MAXU, S_MINV, S_MAXV, S_TA0,
    S_LCM, S_LODL, S_MMAG, S_MMINLIN, S_HASCLUT, S_TEXA0, S_TEXA1, S_COUNT
};
int32_t gs_state[S_COUNT];
double gs_lodk;
/* Vertices: x, y, z, r, g, b, a, q, s, t, u, v (12 doubles each, up to 3). */
double gs_vertices[3 * 12];
uint32_t gs_pixels;

uint32_t *gs_clut(void) { return clut; }
uint8_t *gs_memory(void) { return memory; }
uint8_t *gs_upload_buffer(void) { return upload_buffer; }
uint8_t *gs_out(void) { return out_rgba; }
int32_t *gs_state_address(void) { return gs_state; }
double *gs_lodk_address(void) { return &gs_lodk; }
double *gs_vertices_address(void) { return gs_vertices; }
int32_t *gs_transfer_address(void) { return gs_transfer; }
uint32_t gs_pixel_count(void) { return gs_pixels; }

static uint32_t expand16(uint32_t value) {
    const uint32_t r = (value & 31) << 3, g = ((value >> 5) & 31) << 3, b = ((value >> 10) & 31) << 3;
    return r | g << 8 | b << 16 | ((uint32_t)(value & 0x8000 ? gs_state[S_TEXA1] : gs_state[S_TEXA0]) << 24);
}

/* JavaScript ToInt32 of a double (what `x | 0` does). */
static inline int32_t to_int32(double v) {
    if (v > -2147483649.0 && v < 2147483648.0) return (int32_t)v; /* in range: truncation */
    if (!isfinite(v)) return 0;
    double t = trunc(v);
    if (t >= -2147483648.0 && t <= 2147483647.0) return (int32_t)t;
    t = fmod(t, 4294967296.0);
    if (t < 0) t += 4294967296.0;
    return (int32_t)(uint32_t)t;
}

/* The registers for the current draw: gs_draw copies gs_state here so the compiler may keep them
   in registers across stores into GS memory (which could otherwise alias them). */
static int32_t draw_state[S_COUNT];
#define STATE ((const int32_t *__restrict)draw_state)

static inline __attribute__((always_inline)) uint32_t sample(int32_t u, int32_t v) {
    const int32_t *__restrict s = STATE;
    switch (s[S_WMS]) {
    case 0: u &= s[S_TW] - 1; break;
    case 1: u = u < 0 ? 0 : u >= s[S_TW] ? s[S_TW] - 1 : u; break;
    case 2: u = u < s[S_MINU] ? s[S_MINU] : u > s[S_MAXU] ? s[S_MAXU] : u; break;
    default: u = (u & s[S_MINU]) | s[S_MAXU];
    }
    switch (s[S_WMT]) {
    case 0: v &= s[S_TH] - 1; break;
    case 1: v = v < 0 ? 0 : v >= s[S_TH] ? s[S_TH] - 1 : v; break;
    case 2: v = v < s[S_MINV] ? s[S_MINV] : v > s[S_MAXV] ? s[S_MAXV] : v; break;
    default: v = (v & s[S_MINV]) | s[S_MAXV];
    }
    const uint32_t raw = read_pixel(s[S_PSM], (uint32_t)s[S_TBP], (uint32_t)s[S_TBW], u, v);
    uint32_t color;
    if (s[S_INDEXED]) color = s[S_HASCLUT] ? clut[raw & 255] : 0;
    else if (s[S_PSM] == CT16 || s[S_PSM] == CT16S) color = expand16(raw);
    else if (s[S_PSM] == CT24) color = raw | ((uint32_t)s[S_TA0] << 24);
    else color = raw;
    return s[S_TCC] ? color : ((color & 0xFFFFFF) | 0x80000000u);
}

/* Bilinear as in gs-renderer.mjs sampleBilinear. Its weights fu, fv are k/16 (four fraction
   bits), so every intermediate there is an exact binary fraction and the same value comes out of
   integer arithmetic in sixteenths: top16 = 16a + (b - a)ku, bottom16 likewise, and the result
   floor((16 top16 + (bottom16 - top16) kv) / 256), which is never negative. */
static inline __attribute__((always_inline)) uint32_t sample_bilinear(double u, double v) {
    const double us = u - 0.5, vs = v - 0.5;
    const double u0 = floor(us), v0 = floor(vs);
    const int32_t ku = (int32_t)floor((us - u0) * 16), kv = (int32_t)floor((vs - v0) * 16);
    const int32_t iu = to_int32(u0), iv = to_int32(v0);
    /* On a texel centre (both fractions zero) the filter returns that texel exactly, and with one
       fraction zero it blends two: full-screen copies and HUD sprites are texel-aligned. */
    if (kv == 0) {
        const uint32_t a0 = sample(iu, iv);
        if (ku == 0) return a0;
        const uint32_t b0 = sample(iu + 1, iv);
        uint32_t out = 0;
        for (int shift = 0; shift < 32; shift += 8) {
            const int32_t a = (a0 >> shift) & 255, b = (b0 >> shift) & 255;
            out |= ((uint32_t)((16 * (16 * a + (b - a) * ku)) >> 8) & 255) << shift;
        }
        return out;
    }
    const uint32_t c00 = sample(iu, iv), c10 = sample(iu + 1, iv), c01 = sample(iu, iv + 1), c11 = sample(iu + 1, iv + 1);
    uint32_t out = 0;
    for (int shift = 0; shift < 32; shift += 8) {
        const int32_t a = (c00 >> shift) & 255, b = (c10 >> shift) & 255, c = (c01 >> shift) & 255, d = (c11 >> shift) & 255;
        const int32_t top = 16 * a + (b - a) * ku, bottom = 16 * c + (d - c) * ku;
        out |= ((uint32_t)((16 * top + (bottom - top) * kv) >> 8) & 255) << shift;
    }
    return out;
}

/* The pixel being drawn. */
static double P_R, P_G, P_B, P_A, P_Z, P_U, P_V, P_S, P_T, P_Q = 1;

static inline __attribute__((always_inline)) void pixel_at(int32_t x, int32_t y) {
    const int32_t *__restrict s = STATE;
    if (x < s[S_SCX0] || x > s[S_SCX1] || y < s[S_SCY0] || y > s[S_SCY1]) return;
    int32_t r = to_int32(P_R), g = to_int32(P_G), b = to_int32(P_B), a = to_int32(P_A);
    if (s[S_TME]) {
        double uf, vf, q = 1;
        if (s[S_FST]) { uf = P_U; vf = P_V; }
        else { q = (P_Q != 0 && P_Q == P_Q) ? P_Q : 1; /* `p.q || 1` */ uf = (P_S / q) * s[S_TW]; vf = (P_T / q) * s[S_TH]; }
        int linear = s[S_MMAG];
        if (s[S_MMAG] != s[S_MMINLIN]) { /* the level of detail only matters when the two filters differ */
            const double lod = s[S_LCM] ? gs_lodk : (s[S_FST] ? 0 : -log2(fabs(q)) * (1 << s[S_LODL])) + gs_lodk;
            linear = lod <= 0 ? s[S_MMAG] : s[S_MMINLIN];
        }
        const uint32_t c = linear ? sample_bilinear(uf, vf) : sample(to_int32(floor(uf)), to_int32(floor(vf)));
        const int32_t tr = c & 255, tg = (c >> 8) & 255, tb = (c >> 16) & 255, ta = (int32_t)(c >> 24);
        switch (s[S_TFX]) {
        case 0: r = (tr * r) >> 7; g = (tg * g) >> 7; b = (tb * b) >> 7; a = s[S_TCC] ? (ta * a) >> 7 : a; break;
        case 1: r = tr; g = tg; b = tb; a = s[S_TCC] ? ta : a; break;
        case 2: r = ((tr * r) >> 7) + a; g = ((tg * g) >> 7) + a; b = ((tb * b) >> 7) + a; a = s[S_TCC] ? ta + a : a; break;
        default: r = ((tr * r) >> 7) + a; g = ((tg * g) >> 7) + a; b = ((tb * b) >> 7) + a; a = s[S_TCC] ? ta : a;
        }
        if (r > 255) r = 255; if (g > 255) g = 255; if (b > 255) b = 255; if (a > 255) a = 255;
    }
    int write_color = 1, write_z = 1;
    if (s[S_ATE]) {
        const int32_t aref = s[S_AREF];
        int pass;
        switch (s[S_ATST]) {
        case 0: pass = 0; break; case 1: pass = 1; break; case 2: pass = a < aref; break; case 3: pass = a <= aref; break;
        case 4: pass = a == aref; break; case 5: pass = a >= aref; break; case 6: pass = a > aref; break; default: pass = a != aref;
        }
        if (!pass) {
            if (s[S_AFAIL] == 0) return;
            if (s[S_AFAIL] == 2) write_color = 0; else write_z = 0;
        }
    }
    const double zd = P_Z <= 0 ? 0 : P_Z >= 4294967295.0 ? 4294967295.0 : floor(P_Z);
    const uint32_t z = (uint32_t)zd;
    if (s[S_FPSM] == CT32 && !s[S_FB16] && (!s[S_ZTE] || s[S_ZPSM] == Z32 || s[S_ZPSM] == Z24)) {
        /* The common case: a 32-bit frame and a 32/24-bit Z buffer of the same width share the
           swizzle, so one in-page offset serves both (the same results as the general path). */
        const uint32_t page = (uint32_t)(((y >> 5) * s[S_FBW] + (x >> 6)) * 2048 + SWZ32[((y & 31) << 6) | (x & 63)]);
        uint32_t *zp = (uint32_t *)(memory + (((uint32_t)s[S_ZBP] * 64 + page) * 4 & MASK & ~3u));
        if (s[S_ZTE]) {
            if (s[S_ZTST] == 0) return;
            if (s[S_ZTST] >= 2) {
                const uint32_t current = s[S_ZPSM] == Z24 ? *zp & 0xFFFFFF : *zp;
                if (s[S_ZTST] == 2 ? z < current : z <= current) return;
            }
        }
        uint32_t *fp = (uint32_t *)(memory + (((uint32_t)s[S_FBP] * 64 + page) * 4 & MASK & ~3u));
        if (write_color) {
            if (s[S_ABE] && !(s[S_PABE] && a < 0x80)) {
                const uint32_t d = *fp;
                const int32_t dr = d & 255, dg = (d >> 8) & 255, db = (d >> 16) & 255;
                const int32_t cv = s[S_CSEL] == 0 ? a : s[S_CSEL] == 1 ? (int32_t)(d >> 24) : s[S_FIX];
#define PA(sel, src, dst2) ((sel) == 0 ? (src) : (sel) == 1 ? (dst2) : 0)
                const int32_t nr = (((PA(s[S_ASEL], r, dr) - PA(s[S_BSEL], r, dr)) * cv) >> 7) + PA(s[S_DSEL], r, dr);
                const int32_t ng = (((PA(s[S_ASEL], g, dg) - PA(s[S_BSEL], g, dg)) * cv) >> 7) + PA(s[S_DSEL], g, dg);
                const int32_t nb = (((PA(s[S_ASEL], b, db) - PA(s[S_BSEL], b, db)) * cv) >> 7) + PA(s[S_DSEL], b, db);
#undef PA
                r = nr < 0 ? 0 : nr > 255 ? 255 : nr; g = ng < 0 ? 0 : ng > 255 ? 255 : ng; b = nb < 0 ? 0 : nb > 255 ? 255 : nb;
            }
            const uint32_t value = (uint32_t)(r | g << 8 | b << 16 | ((a | s[S_FBA]) & 0xFF) << 24), mask = (uint32_t)s[S_FBMSK];
            *fp = mask ? (*fp & mask) | (value & ~mask) : value;
            gs_pixels += 1;
        }
        if (write_z && !s[S_ZMSK] && s[S_ZTE]) *zp = s[S_ZPSM] == Z24 ? (*zp & 0xFF000000u) | (z & 0xFFFFFF) : z;
        return;
    }
    if (s[S_ZTE]) {
        if (s[S_ZTST] == 0) return;
        if (s[S_ZTST] >= 2) {
            const uint32_t current = read_pixel(s[S_ZPSM], (uint32_t)s[S_ZBP], (uint32_t)s[S_FBW], x, y);
            if (s[S_ZTST] == 2 ? z < current : z <= current) return;
        }
    }
    if (write_color) {
        if (s[S_ABE] && !(s[S_PABE] && a < 0x80)) {
            const uint32_t dst = read_pixel(s[S_FB16] ? s[S_FPSM] : CT32, (uint32_t)s[S_FBP], (uint32_t)s[S_FBW], x, y);
            const uint32_t d = s[S_FB16] ? expand16(dst) : dst;
            const int32_t dr = d & 255, dg = (d >> 8) & 255, db = (d >> 16) & 255;
            const int32_t cv = s[S_CSEL] == 0 ? a : s[S_CSEL] == 1 ? (int32_t)(d >> 24) : s[S_FIX];
#define PA(sel, src, dst2) ((sel) == 0 ? (src) : (sel) == 1 ? (dst2) : 0)
            const int32_t nr = (((PA(s[S_ASEL], r, dr) - PA(s[S_BSEL], r, dr)) * cv) >> 7) + PA(s[S_DSEL], r, dr);
            const int32_t ng = (((PA(s[S_ASEL], g, dg) - PA(s[S_BSEL], g, dg)) * cv) >> 7) + PA(s[S_DSEL], g, dg);
            const int32_t nb = (((PA(s[S_ASEL], b, db) - PA(s[S_BSEL], b, db)) * cv) >> 7) + PA(s[S_DSEL], b, db);
#undef PA
            r = nr < 0 ? 0 : nr > 255 ? 255 : nr; g = ng < 0 ? 0 : ng > 255 ? 255 : ng; b = nb < 0 ? 0 : nb > 255 ? 255 : nb;
        }
        if (s[S_FB16]) write_pixel(s[S_FPSM], (uint32_t)s[S_FBP], (uint32_t)s[S_FBW], x, y, (uint32_t)((r >> 3) | (g >> 3) << 5 | (b >> 3) << 10 | (a & 0x80 ? 0x8000 : 0)), 0);
        else write_pixel(s[S_FPSM], (uint32_t)s[S_FBP], (uint32_t)s[S_FBW], x, y, (uint32_t)(r | g << 8 | b << 16 | ((a | s[S_FBA]) & 0xFF) << 24), (uint32_t)s[S_FBMSK]);
        gs_pixels += 1;
    }
    if (write_z && !s[S_ZMSK] && s[S_ZTE]) write_pixel(s[S_ZPSM], (uint32_t)s[S_ZBP], (uint32_t)s[S_FBW], x, y, z, 0);
}

#define VX(i) gs_vertices[(i) * 12 + 0]
#define VY(i) gs_vertices[(i) * 12 + 1]
#define VZ(i) gs_vertices[(i) * 12 + 2]
#define VR(i) gs_vertices[(i) * 12 + 3]
#define VG(i) gs_vertices[(i) * 12 + 4]
#define VB(i) gs_vertices[(i) * 12 + 5]
#define VA(i) gs_vertices[(i) * 12 + 6]
#define VQ(i) gs_vertices[(i) * 12 + 7]
#define VS(i) gs_vertices[(i) * 12 + 8]
#define VT(i) gs_vertices[(i) * 12 + 9]
#define VU(i) gs_vertices[(i) * 12 + 10]
#define VV(i) gs_vertices[(i) * 12 + 11]

static inline __attribute__((always_inline)) void mix(int a, int b, int c, double wa, double wb, double wc) {
    P_Z = VZ(a) * wa + VZ(b) * wb + VZ(c) * wc;
    if (STATE[S_IIP]) { P_R = VR(a) * wa + VR(b) * wb + VR(c) * wc; P_G = VG(a) * wa + VG(b) * wb + VG(c) * wc; P_B = VB(a) * wa + VB(b) * wb + VB(c) * wc; P_A = VA(a) * wa + VA(b) * wb + VA(c) * wc; }
    else { P_R = VR(c); P_G = VG(c); P_B = VB(c); P_A = VA(c); }
    if (STATE[S_FST]) { P_U = VU(a) * wa + VU(b) * wb + VU(c) * wc; P_V = VV(a) * wa + VV(b) * wb + VV(c) * wc; }
    else { P_S = VS(a) * wa + VS(b) * wb + VS(c) * wc; P_T = VT(a) * wa + VT(b) * wb + VT(c) * wc; P_Q = VQ(a) * wa + VQ(b) * wb + VQ(c) * wc; }
}

static double dmin(double a, double b) { return a < b ? a : b; }
static double dmax(double a, double b) { return a > b ? a : b; }

/* type: 0 point, 1-2 line, 3-5 triangle, 6 sprite; vertices 0..n-1 in gs_vertices. */
void gs_draw(int32_t type) {
    for (int i = 0; i < S_COUNT; i++) draw_state[i] = gs_state[i];
    const int32_t *__restrict s = STATE;
    if (type == 6) {
        const int a = 0, b = 1;
        P_R = VR(b); P_G = VG(b); P_B = VB(b); P_A = VA(b); P_Z = VZ(b); P_Q = VQ(b);
        const double x0 = dmax(ceil(dmin(VX(a), VX(b))), s[S_SCX0]), x1 = dmin(dmax(VX(a), VX(b)), s[S_SCX1] + 1);
        const double y0 = dmax(ceil(dmin(VY(a), VY(b))), s[S_SCY0]), y1 = dmin(dmax(VY(a), VY(b)), s[S_SCY1] + 1);
        const double dx = (VX(b) - VX(a)) != 0 ? VX(b) - VX(a) : 1, dy = (VY(b) - VY(a)) != 0 ? VY(b) - VY(a) : 1;
        for (double y = y0; y < y1; y++) {
            const double fy = (y - VY(a)) / dy;
            if (s[S_FST]) P_V = VV(a) + (VV(b) - VV(a)) * fy; else P_T = VT(a) + (VT(b) - VT(a)) * fy;
            const double du = (s[S_FST] ? VU(b) - VU(a) : VS(b) - VS(a)) / dx;
            double uu = (s[S_FST] ? VU(a) : VS(a)) + du * (x0 - VX(a));
            for (double x = x0; x < x1; x++, uu += du) {
                if (s[S_FST]) P_U = uu; else P_S = uu;
                pixel_at((int32_t)x, (int32_t)y);
            }
        }
    } else if (type >= 3) {
        const int a = 0, b = 1, c = 2;
        const double area = (VX(b) - VX(a)) * (VY(c) - VY(a)) - (VX(c) - VX(a)) * (VY(b) - VY(a));
        if (area == 0) return;
        const double inv = 1 / area;
        const double minX = dmax(floor(dmin(dmin(VX(a), VX(b)), VX(c))), s[S_SCX0]), maxX = dmin(ceil(dmax(dmax(VX(a), VX(b)), VX(c))), s[S_SCX1]);
        const double minY = dmax(floor(dmin(dmin(VY(a), VY(b)), VY(c))), s[S_SCY0]), maxY = dmin(ceil(dmax(dmax(VY(a), VY(b)), VY(c))), s[S_SCY1]);
        /* Barycentric weights are linear in x: step them (and every attribute) along the row
           rather than recomputing them. w2 = 1 - w0 - w1, so P = Pc + (Pa - Pc) w0 + (Pb - Pc) w1. */
        const double d0 = (VY(b) - VY(c)) * inv, d1 = (VY(c) - VY(a)) * inv;
        const int iip = s[S_IIP], fst = s[S_FST];
#define STEP(V) const double V##c = V(c), V##a = V(a) - V##c, V##b = V(b) - V##c, V##dx = V##a * d0 + V##b * d1;
        STEP(VZ) STEP(VR) STEP(VG) STEP(VB) STEP(VA) STEP(VU) STEP(VV) STEP(VS) STEP(VT) STEP(VQ)
#undef STEP
        if (!iip) { P_R = VR(c); P_G = VG(c); P_B = VB(c); P_A = VA(c); }
        for (double y = minY; y <= maxY; y++) {
            const double py = y + 0.5, px = minX + 0.5;
            double w0 = ((VX(b) - px) * (VY(c) - py) - (VX(c) - px) * (VY(b) - py)) * inv;
            double w1 = ((VX(c) - px) * (VY(a) - py) - (VX(a) - px) * (VY(c) - py)) * inv;
            double z = VZc + VZa * w0 + VZb * w1, r = VRc + VRa * w0 + VRb * w1, g = VGc + VGa * w0 + VGb * w1;
            double bl = VBc + VBa * w0 + VBb * w1, al = VAc + VAa * w0 + VAb * w1;
            double u = VUc + VUa * w0 + VUb * w1, v = VVc + VVa * w0 + VVb * w1;
            double ss = VSc + VSa * w0 + VSb * w1, tt = VTc + VTa * w0 + VTb * w1, qq = VQc + VQa * w0 + VQb * w1;
            int inside = 0;
            for (double x = minX; x <= maxX; x++) {
                const double w2 = 1 - w0 - w1;
                if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                    inside = 1;
                    P_Z = z;
                    if (iip) { P_R = r; P_G = g; P_B = bl; P_A = al; }
                    if (fst) { P_U = u; P_V = v; } else { P_S = ss; P_T = tt; P_Q = qq; }
                    pixel_at((int32_t)x, (int32_t)y);
                } else if (inside) break; /* a triangle's row is one run */
                w0 += d0; w1 += d1; z += VZdx;
                if (iip) { r += VRdx; g += VGdx; bl += VBdx; al += VAdx; }
                if (fst) { u += VUdx; v += VVdx; } else { ss += VSdx; tt += VTdx; qq += VQdx; }
            }
        }
    } else if (type == 0) {
        P_R = VR(0); P_G = VG(0); P_B = VB(0); P_A = VA(0); P_Z = VZ(0);
        if (s[S_FST]) { P_U = VU(0); P_V = VV(0); } else { P_S = VS(0); P_T = VT(0); P_Q = VQ(0); }
        pixel_at((int32_t)floor(VX(0)), (int32_t)floor(VY(0)));
    } else {
        const int a = 0, b = 1;
        const double steps = dmax(dmax(fabs(VX(b) - VX(a)), fabs(VY(b) - VY(a))), 1);
        for (double i = 0; i <= steps; i++) {
            const double f = i / steps;
            mix(a, b, b, 1 - f, f, 0);
            pixel_at((int32_t)floor(VX(a) + (VX(b) - VX(a)) * f), (int32_t)floor(VY(a) + (VY(b) - VY(a)) * f));
        }
    }
}

/* The displayed picture as RGBA (alpha opaque) in gs_out. */
void gs_framebuffer(uint32_t fbp, uint32_t fbw, int32_t psm, int32_t dbx, int32_t dby, int32_t width, int32_t height) {
    for (int32_t y = 0; y < height; y++) {
        for (int32_t x = 0; x < width; x++) {
            const uint32_t raw = read_pixel(psm, fbp, fbw, x + dbx, y + dby);
            const uint32_t c = psm == CT16 || psm == CT16S ? expand16(raw) : raw;
            uint8_t *o = out_rgba + (y * width + x) * 4;
            o[0] = c & 255; o[1] = (c >> 8) & 255; o[2] = (c >> 16) & 255; o[3] = 255;
        }
    }
}

/* The browser world renderer's picture (world-renderer.mjs) into the frame buffer being drawn:
   width x height RGBA in the upload buffer, rows bottom-up (WebGL), written where alpha is
   nonzero (the world drew there) with alpha 0x80, so what the game drew before (the sky) stays
   behind and what it draws after (effects, HUD) lands on top. */
void gs_blit_rgba(uint32_t fbp, uint32_t fbw, int32_t psm, int32_t width, int32_t height) {
    for (int32_t y = 0; y < height; y++) {
        const uint8_t *row = upload_buffer + (uint32_t)(height - 1 - y) * (uint32_t)width * 4;
        for (int32_t x = 0; x < width; x++) {
            const uint8_t *p = row + x * 4;
            if (!p[3]) continue;
            if (psm == CT16 || psm == CT16S)
                write_pixel(psm, fbp, fbw, x, y, (uint32_t)((p[0] >> 3) | (p[1] >> 3) << 5 | (p[2] >> 3) << 10 | 0x8000), 0);
            else
                write_pixel(psm, fbp, fbw, x, y, (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | 0x80u << 24, 0);
        }
    }
}

/* The browser-drawn world's depth into the Z buffer (zbp in blocks, fbw, zpsm), where the console's
   VU1 world would have written it: upload_buffer holds width x height RGBA, rows bottom-up, the GS
   depth in r | g << 8 | b << 16 and a = 0 where no world geometry covers the pixel (left as is).
   The game's own 3D effects then depth-test against the world. */
void gs_blit_z(uint32_t zbp, uint32_t fbw, int32_t zpsm, int32_t width, int32_t height) {
    for (int32_t y = 0; y < height; y++) {
        const uint8_t *row = upload_buffer + (uint32_t)(height - 1 - y) * (uint32_t)width * 4;
        for (int32_t x = 0; x < width; x++) {
            const uint8_t *p = row + x * 4;
            if (!p[3]) continue;
            const uint32_t z = (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16;
            write_pixel(zpsm, zbp, fbw, x, y, z, 0);
        }
    }
}
