/* Portable versions of more boot-ELF helpers that exist only as EE assembly, for the spike build.
   Same rules as browser/level-math.c: each follows the instructions at its address (listed with
   browser/spike/ee-disasm.py) and the comment names them. Results are IEEE single precision; the
   EE FPU and VU0 round toward zero and flush denormals, so the last bit can differ.

   These are linked only by browser/spike/link-closure.sh. Move one into level-math.c, with a
   test, when a gated target needs it. */
#include "types.h"

#define ALIAS(name, label, target) \
    extern __typeof__(name) name##_##label __asm__(#label) __attribute__((alias(#target)));

typedef struct {
    f32 x, y, z, w;
} Vec4;

static Vec4 load_vec(const void *from) {
    Vec4 v;
    __builtin_memcpy(&v, from, sizeof(v));
    return v;
}

static void store_vec(void *to, Vec4 v) { __builtin_memcpy(to, &v, sizeof(v)); }

/* The EE saturates cvt.w.s; wasm's truncation traps out of range. */
static s32 saturating_s32(f32 x) {
    if (x != x || x >= 2147483648.0f) return 0x7FFFFFFF;
    if (x <= -2147483648.0f) return (s32)0x80000000;
    return (s32)x;
}

/* 0x1fa688: sub.s; abs.s; if !(d < pi) d = (pi + pi) - d. */
f32 fast_difference_between_rotations(f32 a, f32 b) __asm__("FUN_001fa688");
f32 fast_difference_between_rotations(f32 a, f32 b) {
    const f32 pi = 3.14159274f; /* 0x40490fdb */
    f32 d = __builtin_fabsf(a - b);
    if (!(d < pi)) d = (pi + pi) - d;
    return d;
}

/* convert_integer_to_float (0x1fa6c0) lives in browser/level-math.c. */

/* 0x1f96f8: f1 = (f32)frames; ACC = 0.25 + 0.25; madd.s f1 = ACC + f1 * [gp-0x7e98]; cvt.w.s.
   gp is 0x166c00 (config/us/rnc1.us.yaml), so the scale is the f32 at 0x15ED68. That address is
   inside the pinned RAM image; nothing in this build writes it yet, so it reads 0 until real
   boot state is loaded. */
extern f32 game_frame_scale __asm__("D_0015ED68");
s32 scale_game_frames(s32 frames) __asm__("FUN_001f96f8");
s32 scale_game_frames(s32 frames) { return saturating_s32(0.5f + (f32)frames * game_frame_scale); }
ALIAS(scale_game_frames, func_001F96F8, FUN_001f96f8)

/* 0x1f97e8: do { *p = value; bytes -= 4; p += 4; } while (bytes > 0). */
void fill_words(u32 *p, u32 value, s32 bytes) __asm__("FUN_001f97e8");
void fill_words(u32 *p, u32 value, s32 bytes) {
    do {
        *p++ = value;
        bytes -= 4;
    } while (bytes > 0);
}
ALIAS(fill_words, func_001F97E8, FUN_001f97e8)
/* config/us/symbol_addrs.txt: FillTransferWords = 0x001F97E8, the same routine by name. */
ALIAS(fill_words, FillTransferWords, FUN_001f97e8)

/* 0x1f9810: do { sq $0, 0(p); bytes -= 16; p += 16; } while (bytes > 0). */
void fast_mem_zero16(void *p, s32 bytes) __asm__("FUN_001f9810");
void fast_mem_zero16(void *p, s32 bytes) {
    u8 *at = p;
    do {
        __builtin_memset(at, 0, 16);
        at += 16;
        bytes -= 16;
    } while (bytes > 0);
}

/* 0x1f9838: forward copy until the destination reaches dst + n, in 16-, 4-, or 1-byte steps
   chosen by the alignment of dst | src | end. A forward byte copy stores the same bytes for the
   lengths each path accepts. The original loops are do-while on pointer equality, so n == 0
   would not terminate there; that is refused here instead of reproduced. */
void copy_memory_aligned(void *dst, void *src, s32 n) __asm__("FUN_001f9838");
void copy_memory_aligned(void *dst, void *src, s32 n) {
    u8 *to = dst;
    const u8 *from = src;
    if (n <= 0) __builtin_trap();
    while (n-- > 0) *to++ = *from++;
}

/* 0x1f9988: vsqrt Q, vf1x; vaddq.x vf1 = vf0 + Q. VU0 takes the root of the magnitude. */
f32 square_root(f32 x) __asm__("FUN_001f9988");
f32 square_root(f32 x) { return __builtin_sqrtf(__builtin_fabsf(x)); }

/* 0x1f99f8: jr ra; sq $0, 0(a0). Clears a whole quadword despite the recovered name. */
void clear_u64_value(void *p) __asm__("FUN_001f99f8");
void clear_u64_value(void *p) { __builtin_memset(p, 0, 16); }

/* 0x1f9a68: vmulx.xyz vf1 = a * scale; w keeps a.w. */
void fast_vec_scale(void *out, void *a, f32 scale) __asm__("FUN_001f9a68");
void fast_vec_scale(void *out, void *a, f32 scale) {
    Vec4 v = load_vec(a);
    v.x *= scale; v.y *= scale; v.z *= scale;
    store_vec(out, v);
}

/* 0x1f9a80: vmulx.xyzw, the same including w. Recovered callers disagree on where the float
   goes in the prototype; this uses (out, a, scale) and the others trap at link-reported sites. */
void scale_vector(void *out, void *a, f32 scale) __asm__("FUN_001f9a80");
void scale_vector(void *out, void *a, f32 scale) {
    Vec4 v = load_vec(a);
    v.x *= scale; v.y *= scale; v.z *= scale; v.w *= scale;
    store_vec(out, v);
}

/* 0x1f9ab0: vmul.xyz; vadday.x ACC = x + y; vmaddz.x = ACC + 1 * z. */
f32 fast_vec_dot(void *a, void *b) __asm__("FUN_001f9ab0");
f32 fast_vec_dot(void *a, void *b) {
    const Vec4 va = load_vec(a);
    const Vec4 vb = load_vec(b);
    return (va.x * vb.x + va.y * vb.y) + va.z * vb.z;
}

/* 0x1f9af0: the same with a twice, then vsqrt. */
f32 fast_vec_length(void *a) __asm__("FUN_001f9af0");
f32 fast_vec_length(void *a) {
    const Vec4 v = load_vec(a);
    return __builtin_sqrtf((v.x * v.x + v.y * v.y) + v.z * v.z);
}

/* 0x1f9b80: vsub.xy; vmul.xy; vaddy.x; vsqrt. */
f32 vector_distance_xy(void *a, void *b) __asm__("FUN_001f9b80");
f32 vector_distance_xy(void *a, void *b) {
    const Vec4 va = load_vec(a);
    const Vec4 vb = load_vec(b);
    const f32 dx = va.x - vb.x;
    const f32 dy = va.y - vb.y;
    return __builtin_sqrtf(dx * dx + dy * dy);
}

/* 0x1f9bf8: s = x*x + y*y + z*z; vrsqrt Q = length / sqrt(s); if the bits of s are zero,
   xyz = 0, else xyz = a * Q. w keeps a.w either way. */
void fast_vec_normalize(void *out, void *a, f32 length) __asm__("FUN_001f9bf8");
void fast_vec_normalize(void *out, void *a, f32 length) {
    Vec4 v = load_vec(a);
    const f32 s = (v.x * v.x + v.y * v.y) + v.z * v.z;
    u32 bits;
    __builtin_memcpy(&bits, &s, 4);
    if (bits == 0) {
        v.x = v.y = v.z = 0.0f;
    } else {
        const f32 q = length / __builtin_sqrtf(s);
        v.x *= q; v.y *= q; v.z *= q;
    }
    store_vec(out, v);
}
/* The level program's copy at 0x1ff500 (level 0 text): its 20 instruction words equal these. */
ALIAS(fast_vec_normalize, FUN_L00_001ff500, FUN_001f9bf8)

/* 0x1f9cf8: ACC = m[0]*v.x + m[1]*v.y + m[2]*v.z; vmaddw out = ACC + vf0 * v.w, and vf0 is
   (0, 0, 0, 1), so only w gains v.w. */
void transform_vector_by_basis(void *out, void *v, void *m) __asm__("FUN_001f9cf8");
void transform_vector_by_basis(void *out, void *v, void *m) {
    const Vec4 a = load_vec(v);
    const Vec4 c0 = load_vec(m);
    const Vec4 c1 = load_vec((u8 *)m + 0x10);
    const Vec4 c2 = load_vec((u8 *)m + 0x20);
    Vec4 r;
    r.x = (c0.x * a.x + c1.x * a.y) + c2.x * a.z;
    r.y = (c0.y * a.x + c1.y * a.y) + c2.y * a.z;
    r.z = (c0.z * a.x + c1.z * a.y) + c2.z * a.z;
    r.w = ((c0.w * a.x + c1.w * a.y) + c2.w * a.z) + a.w;
    store_vec(out, r);
}
ALIAS(transform_vector_by_basis, func_001F9CF8, FUN_001f9cf8)

/* 0x1f9d20: the same with the fourth column: out = ACC + m[3] * v.w. */
void transform_vector(void *out, void *v, void *m) __asm__("FUN_001f9d20");
void transform_vector(void *out, void *v, void *m) {
    const Vec4 a = load_vec(v);
    const Vec4 c0 = load_vec(m);
    const Vec4 c1 = load_vec((u8 *)m + 0x10);
    const Vec4 c2 = load_vec((u8 *)m + 0x20);
    const Vec4 c3 = load_vec((u8 *)m + 0x30);
    Vec4 r;
    r.x = ((c0.x * a.x + c1.x * a.y) + c2.x * a.z) + c3.x * a.w;
    r.y = ((c0.y * a.x + c1.y * a.y) + c2.y * a.z) + c3.y * a.w;
    r.z = ((c0.z * a.x + c1.z * a.y) + c2.z * a.z) + c3.z * a.w;
    r.w = ((c0.w * a.x + c1.w * a.y) + c2.w * a.z) + c3.w * a.w;
    store_vec(out, r);
}

/* 0x1f9dc8 / 0x1f9de0: qmtc2 the angle into vf1 and vcallms a VU0 microprogram. The microprogram
   is NOT decoded here. These call the host's cos/sin instead (imported as cosf/sinf by the
   runner), the same stand-in browser/hero.c uses; equivalence to the console's approximation is
   unverified. */
extern f32 cosf(f32);
extern f32 sinf(f32);
f32 fast_cos(f32 radians) __asm__("FUN_001f9dc8");
f32 fast_cos(f32 radians) { return cosf(radians); }
ALIAS(fast_cos, func_001F9DC8, FUN_001f9dc8)
f32 fast_sin(f32 radians) __asm__("FUN_001f9de0");
f32 fast_sin(f32 radians) { return sinf(radians); }
ALIAS(fast_sin, func_001F9DE0, FUN_001f9de0)

/* 0x1f9df8: s = 1.0, or -1.0 with x = |x| when x < 0; vsqrt Q = sqrt(1 - x);
   ACC = c0 (adda.s), then madda.s x * c1, madda.s x^2 * c2 (the two raw words 0x4605601e and
   0x4606101e), madd.s p = ACC + x^3 * c3; result s * (pi/2 - p * Q). The four coefficients are
   read from gp - 0x6560 (gp = 0x166c00, config/us/rnc1.us.yaml), bits 0x3fc90da4 0xbe593484
   0x3d981627 0xbc996e30 in the boot ELF. vsqrt takes the root of |ft|, so |x| > 1 gives
   sqrt(|1 - x|), not NaN. pi/2 is the lui/ori pair 0x3fc90fdb. */
f32 fast_arc_sin(f32 x) __asm__("FUN_001f9df8");
f32 fast_arc_sin(f32 x) {
    static const u32 coefficient_bits[4] = { 0x3fc90da4, 0xbe593484, 0x3d981627, 0xbc996e30 };
    f32 c[4];
    f32 sign = 1.0f;
    f32 polynomial;
    __builtin_memcpy(c, coefficient_bits, sizeof(c));
    if (x < 0.0f) {
        sign = -1.0f;
        x = __builtin_fabsf(x);
    }
    polynomial = ((c[0] + x * c[1]) + (x * x) * c[2]) + (x * (x * x)) * c[3];
    return sign * (1.57079637f - polynomial * __builtin_sqrtf(__builtin_fabsf(1.0f - x)));
}
ALIAS(fast_arc_sin, func_001F9DF8, FUN_001f9df8)
/* The level program's copy at 0x1ff818 (level 0 text) differs only in its branch target and in
   reading the same four coefficient words from gp - 0x64a0 (0x160760 in the level's lit). */
ALIAS(fast_arc_sin, FUN_L00_001ff818, FUN_001f9df8)

/* 0x1f99c8: max.s. */
f32 max_float(f32 a, f32 b) __asm__("FUN_001f99c8");
f32 max_float(f32 a, f32 b) { return a > b ? a : b; }

/* 0x1f9a00: vmr32 vf1 = (vf0.y, vf0.z, vf0.w, vf0.x) = (0, 0, 1, 0); sqc2. */
void set_vector_unit_z(void *out) __asm__("FUN_001f9a00");
void set_vector_unit_z(void *out) {
    const Vec4 z = { 0.0f, 0.0f, 1.0f, 0.0f };
    store_vec(out, z);
}

/* 0x1f9a10: vadd.xyz vf1 = a + b; w keeps a.w. */
void fast_vec_add(void *out, void *a, void *b) __asm__("FUN_001f9a10");
void fast_vec_add(void *out, void *a, void *b) {
    Vec4 va = load_vec(a);
    const Vec4 vb = load_vec(b);
    va.x += vb.x; va.y += vb.y; va.z += vb.z;
    store_vec(out, va);
}

/* 0x1f9ad8: vopmula ACC = b.yzx * a.zxy (fs = vf2 = b, ft = vf1 = a);
   vopmsub vf3 = ACC - a.yzx * b.zxy. That is b cross a. vf3.w is whatever VU0 last left there;
   it is written as 0 here, and no decoded caller has been checked to read it. */
void fast_vec_cross(void *out, void *a, void *b) __asm__("FUN_001f9ad8");
void fast_vec_cross(void *out, void *a, void *b) {
    const Vec4 va = load_vec(a);
    const Vec4 vb = load_vec(b);
    Vec4 r;
    r.x = vb.y * va.z - va.y * vb.z;
    r.y = vb.z * va.x - va.z * vb.x;
    r.z = vb.x * va.y - va.x * vb.y;
    r.w = 0.0f;
    store_vec(out, r);
}

/* 0x1f98d0: do { lq/sq one quadword; n -= 16; } while (n > 0). */
void fast_mem_copy(void *dst, void *src, s32 n) __asm__("FUN_001f98d0");
void fast_mem_copy(void *dst, void *src, s32 n) {
    u8 *to = dst;
    const u8 *from = src;
    do {
        u8 quad[16];
        __builtin_memcpy(quad, from, 16);
        __builtin_memcpy(to, quad, 16);
        to += 16; from += 16; n -= 16;
    } while (n > 0);
}

/* 0x1f9740: t = *timer; if (t == 0) return 1; t = max(t, 1) - 1 (pmaxw in the delay slot);
   *timer = t; return t > 0 ? 0 : 2. */
s32 fast_dec_timer(s32 *timer) __asm__("FUN_001f9740");
s32 fast_dec_timer(s32 *timer) {
    s32 t = *timer;
    if (t == 0) return 1;
    t = (t > 1 ? t : 1) - 1;
    *timer = t;
    return t > 0 ? 0 : 2;
}

/* 0x1fa2b8: copy three quadwords. */
void copy_matrix3x4(void *out, void *in) __asm__("FUN_001fa2b8");
void copy_matrix3x4(void *out, void *in) { __builtin_memmove(out, in, 0x30); }

/* 0x1fa298: the same, then sqc2 vf0 = (0, 0, 0, 1) at +0x30. */
void copy_matrix_rotation(void *out, void *in) __asm__("FUN_001fa298");
void copy_matrix_rotation(void *out, void *in) {
    const Vec4 w = { 0.0f, 0.0f, 0.0f, 1.0f };
    __builtin_memmove(out, in, 0x30);
    store_vec((u8 *)out + 0x30, w);
}

/* 0x1fa2d8: out[3] = (0, 0, 0, 1) first; then row i of out = column i of in for the 3x3 part,
   each w = vf0.w - vf0.w = 0. All three inputs are loaded before any store. */
void transpose_rotation_matrix(void *out, void *in) __asm__("FUN_001fa2d8");
void transpose_rotation_matrix(void *out, void *in) {
    const Vec4 m0 = load_vec(in);
    const Vec4 m1 = load_vec((u8 *)in + 0x10);
    const Vec4 m2 = load_vec((u8 *)in + 0x20);
    const Vec4 w = { 0.0f, 0.0f, 0.0f, 1.0f };
    const Vec4 r0 = { m0.x, m1.x, m2.x, 0.0f };
    const Vec4 r1 = { m0.y, m1.y, m2.y, 0.0f };
    const Vec4 r2 = { m0.z, m1.z, m2.z, 0.0f };
    store_vec((u8 *)out + 0x30, w);
    store_vec(out, r0);
    store_vec((u8 *)out + 0x10, r1);
    store_vec((u8 *)out + 0x20, r2);
}

#include "moby_animation_advance.h"


extern s32 allocate_voice_for_target_entry(s32, s32, void *) __asm__("FUN_0022da68");
extern void update_moby_voice(void *) __asm__("FUN_0020c940");
extern const s32 world_update_time __asm__("D_0015F60C"); /* gp - 0x75f4 */
void advance_moby_animation(void *moby) __asm__("FUN_0020d580");
void advance_moby_animation(void *moby) {
    rnc_advance_moby_animation(moby, world_update_time, allocate_voice_for_target_entry, update_moby_voice);
}
ALIAS(advance_moby_animation, func_0020D580, FUN_0020d580)

/* Level 0's copy: callees 0x28dbe8 and 0x24ec80 (the level's copies of the two above, not
   recovered), counter gp - 0x7634. */
extern s32 level_allocate_voice_for_target_entry(s32, s32, void *) __asm__("FUN_L00_0028dbe8");
extern void level_update_moby_voice(void *) __asm__("FUN_L00_0024ec80");
extern const s32 level_frame_counter __asm__("D_L00_0015F5CC");
void level_advance_moby_animation(void *moby) __asm__("FUN_L00_00250480");
void level_advance_moby_animation(void *moby) {
    rnc_advance_moby_animation(moby, level_frame_counter, level_allocate_voice_for_target_entry,
                                level_update_moby_voice);
}

/* 0x1fa3c0: a = *a1, b = *a2 (listing order fd, fs, ft, checked against the raw fields).
   vmulw.xyz b * a.w and a * b.w; vopmula/vopmsub a x b; vadday/vmaddz d = (b.x*a.x + b.y*a.y)
   + b.z*a.z. out.xyz = (b * a.w + a * b.w) + a x b; out.w = a.w * b.w - d. The quaternion
   product a * b with w last. */
void multiply_quaternions(void *out, void *a_in, void *b_in) __asm__("FUN_001fa3c0");
void multiply_quaternions(void *out, void *a_in, void *b_in) {
    const Vec4 a = load_vec(a_in);
    const Vec4 b = load_vec(b_in);
    const f32 d = (b.x * a.x + b.y * a.y) + b.z * a.z;
    Vec4 r;
    r.x = (b.x * a.w + a.x * b.w) + (a.y * b.z - b.y * a.z);
    r.y = (b.y * a.w + a.y * b.w) + (a.z * b.x - b.z * a.x);
    r.z = (b.z * a.w + a.z * b.w) + (a.x * b.y - b.x * a.y);
    r.w = b.w * a.w - d;
    store_vec(out, r);
}
ALIAS(multiply_quaternions, func_001FA3C0, FUN_001fa3c0)

/* 0x212c28 find_camera_environment_region(position, &blend, &index). p = position with w = 1.
   For i = 0, 1, ... up to and including the count at 0x15fb44 (the `bne` compares the element
   just tested, before the delay-slot increment), sphere s = 0x19abc0 + 16 * i; the region is hit
   when (p.x - s.x)^2 + (p.y - s.y)^2 - s.w has its sign bit set (x and y only: vaddy.x). On the
   first hit only: v = c0 * p.x + c1 * p.y + c2 * p.z + c3 (columns at 0x19adc0 + 0x80 * i,
   D_0019ADC0 in update_camera_environment_from_regions.c). vclipw.xyz against vf0.w = 1 sets a
   flag where v exceeds +1 or is below -1; any flag returns 0 without trying later regions.
   Otherwise *index = i, *blend = 0.5 * (v.x + 1), and 1 is returned. No hit returns 0.
   A negative count is not reproduced: the console's do-while would test element 0 and then
   never meet its end pointer; here nothing is tested. */
extern const s32 camera_environment_region_last __asm__("D_0015FB44");
extern const u8 camera_environment_spheres[] __asm__("D_0019ABC0");
extern const u8 camera_environment_region_records[] __asm__("D_0019ADC0");
s32 find_camera_environment_region(void *position, f32 *blend, s32 *index) __asm__("FUN_00212c28");
s32 find_camera_environment_region(void *position, f32 *blend, s32 *index) {
    Vec4 p = load_vec(position);
    s32 i;
    p.w = 1.0f;
    for (i = 0; i <= camera_environment_region_last; i++) {
        const Vec4 s = load_vec(camera_environment_spheres + 16 * i);
        const f32 dx = p.x - s.x;
        const f32 dy = p.y - s.y;
        const f32 inside = (dx * dx + dy * dy) - s.w;
        u32 bits;
        __builtin_memcpy(&bits, &inside, 4);
        if (bits >> 31) {
            const u8 *columns = camera_environment_region_records + 0x80 * i;
            const Vec4 c0 = load_vec(columns), c1 = load_vec(columns + 0x10);
            const Vec4 c2 = load_vec(columns + 0x20), c3 = load_vec(columns + 0x30);
            const f32 vx = ((c0.x * p.x + c1.x * p.y) + c2.x * p.z) + c3.x * p.w;
            const f32 vy = ((c0.y * p.x + c1.y * p.y) + c2.y * p.z) + c3.y * p.w;
            const f32 vz = ((c0.z * p.x + c1.z * p.y) + c2.z * p.z) + c3.z * p.w;
            if (vx > 1.0f || vx < -1.0f || vy > 1.0f || vy < -1.0f || vz > 1.0f || vz < -1.0f) {
                return 0;
            }
            *index = i;
            *blend = 0.5f * (vx + 1.0f);
            return 1;
        }
    }
    return 0;
}
ALIAS(find_camera_environment_region, func_00212C28, FUN_00212c28)

/* 0x1fa378: load the four columns of m; for each of the four vectors v of n, in order,
   out[i] = m0*v.x + m1*v.y + m2*v.z + m3*v.w, stored before the next v is loaded. */
void multiply_matrix(void *out, void *m, void *n) __asm__("FUN_001fa378");
void multiply_matrix(void *out, void *m, void *n) {
    const Vec4 c0 = load_vec(m);
    const Vec4 c1 = load_vec((u8 *)m + 0x10);
    const Vec4 c2 = load_vec((u8 *)m + 0x20);
    const Vec4 c3 = load_vec((u8 *)m + 0x30);
    s32 i;
    for (i = 0; i < 4; i++) {
        const Vec4 v = load_vec((u8 *)n + i * 0x10);
        Vec4 r;
        r.x = ((c0.x * v.x + c1.x * v.y) + c2.x * v.z) + c3.x * v.w;
        r.y = ((c0.y * v.x + c1.y * v.y) + c2.y * v.z) + c3.y * v.w;
        r.z = ((c0.z * v.x + c1.z * v.y) + c2.z * v.z) + c3.z * v.w;
        r.w = ((c0.w * v.x + c1.w * v.y) + c2.w * v.z) + c3.w * v.w;
        store_vec((u8 *)out + i * 0x10, r);
    }
}

/* 0x1fa610: while (!(x < pi)) x = (x - pi) - pi; while (x < -pi) x = (x + pi) + pi. */
f32 fast_normalize_angle(f32 x) __asm__("FUN_001fa610");
f32 fast_normalize_angle(f32 x) {
    const f32 pi = 3.14159274f;
    while (!(x < pi)) x = (x - pi) - pi;
    while (x < -pi) x = (x + pi) + pi;
    return x;
}

/* 0x1f97a0: plzcw a0 (count of leading bits equal to the sign bit, minus one); v0 = 30 - a0.
   For x > 0 that is floor(log2(x)); 0 gives -1; negatives count leading ones. */
s32 log2dim(s32 x) __asm__("FUN_001f97a0");
s32 log2dim(s32 x) {
    const u32 bits = x < 0 ? ~(u32)x : (u32)x;
    const s32 leading = bits == 0 ? 32 : __builtin_clz(bits);
    return 30 - (leading - 1);
}

/* 0x1e93b0: spills the argument registers to its frame and returns. The retail build's debug
   print outputs nothing, so the equivalent body is empty. */
void debug_print(const char *format, ...) __asm__("FUN_001e93b0");
void debug_print(const char *format, ...) { (void)format; }

/* 0x120478 (libgcc fptodp): widen a float to a double. */
f64 fptodp(f32 x) __asm__("FUN_00120478");
f64 fptodp(f32 x) { return (f64)x; }

/* RandomNext: called with no declaration by several recovered files. Inferred, not read from a
   symbol table: src/math/random/random_integer_below.c computes `(rand() >> 16) & 0x7FFF` with
   rand bound to 0x1160d8, and its siblings (random_float_between, random_angle_radians) compute
   the same expression from RandomNext(). Treated as that same routine. */
s32 level_rand(void) __asm__("FUN_001160d8");
s32 RandomNext(void);
s32 RandomNext(void) { return level_rand(); }

/* Second spelling of a helper defined in browser/level-math.c. An alias cannot cross
   translation units, so this forwards. */
void fast_vec_sub(void *out, void *a, void *b) __asm__("FUN_001f9a28");
void fast_vec_sub_second_spelling(void *out, void *a, void *b) __asm__("func_001F9A28");
void fast_vec_sub_second_spelling(void *out, void *a, void *b) { fast_vec_sub(out, a, b); }

/* 0x125180 (SDK sceVu0Normalize): lqc2 vf4,(a1); vf5.x = x*x + y*y + z*z; Q = sqrt(vf5.x);
   Q = 1 / Q; vf6 = 0; vf6.xyz = vf4.xyz * Q; sqc2 vf6,(a0). w is written as 0. A zero-length input
   divides by zero: the VU clamps that to the largest float, IEEE gives inf/NaN here. */
void sce_vu0_normalize(void *out, void *in) __asm__("FUN_00125180");
void sce_vu0_normalize(void *out, void *in) {
    Vec4 v = load_vec(in);
    f32  inverse = 1.0f / __builtin_sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    Vec4 r = { v.x * inverse, v.y * inverse, v.z * inverse, 0.0f };
    store_vec(out, r);
}

/* 0x1f99e8: max.s f0,f12,f13; min.s f0,f0,f14. */
f32 clamp_float(f32 value, f32 low, f32 high) __asm__("FUN_001f99e8");
f32 clamp_float(f32 value, f32 low, f32 high) {
    f32 r = value > low ? value : low;
    return r < high ? r : high;
}

/* 0x1f9770: lh at,(a0); if 0 return 1; pmaxw at,at,1 (a negative count becomes 1); at -= 1;
   sh at,(a0); return at > 0 ? 0 : 2. */
s32 fast_dec_timer_s16(s16 *timer) __asm__("FUN_001f9770");
s32 fast_dec_timer_s16(s16 *timer) {
    s32 t = *timer;
    if (t == 0) return 1;
    if (t < 1) t = 1;
    t -= 1;
    *timer = (s16)t;
    return t > 0 ? 0 : 2;
}

/* Euler angles (x, y, z at +0, +4, +8) to a rotation matrix. Decoded from the EE-only version at
   0x1fa070: start from identity rows; for z, then y, then x (each skipped when its angle bits are
   0), replace every row r by r.x * A + r.y * B + r.z * C, where
     z: A = ( c,  s, 0)  B = (-s, c, 0)  C = (0, 0, 1)
     y: A = ( c,  0, -s) B = ( 0, 1, 0)  C = (s, 0, c)
     x: A = ( 1,  0, 0)  B = ( 0, c, s)  C = (0, -s, c)
   Row w components are 0 and the fourth row is (0, 0, 0, 1). Its sine is a polynomial at
   0x1fa1a8; host sinf/cosf are used instead, as for fast_sin.
   0x1fa030 (three rows) and 0x1fa050 (four rows) hand the same input to a VU0 microprogram that is
   not decoded. ASSUMED to compute the same matrix as the EE version; not compared on a console. */
static void euler_rows(f32 rows[3][4], const void *angles) {
    f32 a[3];
    __builtin_memcpy(a, angles, sizeof(a));
    for (s32 i = 0; i < 3; i++)
        for (s32 j = 0; j < 4; j++) rows[i][j] = i == j ? 1.0f : 0.0f;
    for (s32 axis = 2; axis >= 0; axis--) {
        if (a[axis] == 0.0f) continue; /* the original tests the bit pattern, which also skips -0 */
        f32 s = sinf(a[axis]), c = cosf(a[axis]);
        f32 basis[3][3] = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
        if (axis == 2) { basis[0][0] = c; basis[0][1] = s; basis[1][0] = -s; basis[1][1] = c; }
        if (axis == 1) { basis[0][0] = c; basis[0][2] = -s; basis[2][0] = s; basis[2][2] = c; }
        if (axis == 0) { basis[1][1] = c; basis[1][2] = s; basis[2][1] = -s; basis[2][2] = c; }
        for (s32 i = 0; i < 3; i++) {
            f32 r[3] = { rows[i][0], rows[i][1], rows[i][2] };
            for (s32 j = 0; j < 3; j++) rows[i][j] = r[0] * basis[0][j] + r[1] * basis[1][j] + r[2] * basis[2][j];
        }
    }
}

void euler_rotation_basis(void *out, void *angles) __asm__("FUN_001fa030");
void euler_rotation_basis(void *out, void *angles) {
    f32 rows[3][4];
    euler_rows(rows, angles);
    __builtin_memcpy(out, rows, sizeof(rows));
}

void euler_rotation_matrix(void *out, void *angles) __asm__("FUN_001fa050");
void euler_rotation_matrix(void *out, void *angles) {
    f32 rows[4][4] = { { 0 } };
    euler_rows(rows, angles);
    rows[3][0] = rows[3][1] = rows[3][2] = 0.0f;
    rows[3][3] = 1.0f;
    __builtin_memcpy(out, rows, sizeof(rows));
}

void euler_to_matrix_ee(void *out, void *angles) __asm__("FUN_001fa070");
void euler_to_matrix_ee(void *out, void *angles) { euler_rotation_matrix(out, angles); }
