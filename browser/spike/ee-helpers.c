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
#include "moby_joint_matrices.h"

/* get_moby_joint_matrices (0x210850), decoded in browser/moby-joint-matrices.c. */
void get_moby_joint_matrices(void *moby, s32 count, s32 *bones, void *out) __asm__("FUN_00210850");
void get_moby_joint_matrices(void *moby, s32 count, s32 *bones, void *out) {
    rnc_get_moby_joint_matrices(moby, count, bones, out);
}
ALIAS(get_moby_joint_matrices, func_00210850, FUN_00210850)


extern s32 allocate_voice_for_target_entry(s32, s32, void *) __asm__("FUN_0022da68");
extern void update_moby_voice(void *) __asm__("FUN_0020c940");
extern const s32 world_update_time __asm__("D_0015F60C"); /* gp - 0x75f4 */
/* s32 as include/rnc/mobys/animation/advance_moby_animation.h declares it (no caller reads it). */
s32 advance_moby_animation(void *moby) __asm__("FUN_0020d580");
s32 advance_moby_animation(void *moby) {
    rnc_advance_moby_animation(moby, world_update_time, allocate_voice_for_target_entry, update_moby_voice);
    return 0;
}
ALIAS(advance_moby_animation, func_0020D580, FUN_0020d580)

/* Level 0's copy: callees 0x28dbe8 and 0x24ec80 (the level's copies of the two above, not
   recovered), counter gp - 0x7634. */
/* 0x28dbe8 is level 0's copy of 0x22da68 (level-copies.json), so the boot function is called. */
extern s32 level_allocate_voice_for_target_entry(s32, s32, void *) __asm__("FUN_0022da68");
extern void level_update_moby_voice(void *) __asm__("FUN_L00_0024ec80");
extern const s32 level_frame_counter __asm__("D_0015F60C") /* level copy D_L00_0015F5CC */;
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

/* create_part, level 0 0x2678b8: `j 0x2678c0` with a1 = 0. 0x2678c0, with gp = 0x166c00:
   next = [gp - 0x6a90] (0x160170), highest = [gp - 0x6a8c] (0x160174), table = [gp - 0x6a94]
   (0x16016c), count = [gp - 0x6a88] (0x160178), allocation bits at 0x1b1880 (0x800 particles).
   If next >= 0x800, return 0. Otherwise set next's bit; then if next > highest, next + 1 becomes
   the next free index, else the search starts at the byte holding next + 1 and takes the first
   clear bit (up to 0x800). highest = max(highest, index) (pmaxw, raw word 0x7326c8c8), count + 1.
   The 64-byte particle at table + 64 * index gets its first 32 bytes zeroed (sq 0x0, then 0x10
   three times) and the type in byte 0. The paths at 0x2678e8 and 0x2679c0 are not reached from
   this entry. */
extern s32 particle_next_free __asm__("D_001600B0") /* level copy D_L00_00160170 */;
extern s32 particle_highest __asm__("D_001600B4") /* level copy D_L00_00160174 */;
extern u8 *particle_table __asm__("D_001600AC") /* level copy D_L00_0016016C */;
extern s32 particle_count __asm__("D_001600B8") /* level copy D_L00_00160178 */;
extern u8 particle_allocated[0x100] __asm__("D_001CD780") /* level copy D_L00_001B1880 */;
u8 *create_part(s32 type) __asm__("FUN_L00_002678b8");
u8 *create_part(s32 type) {
    const s32 index = particle_next_free;
    s32 next = index;
    u8 *particle;
    if (index - 0x800 >= 0) return 0;
    particle_allocated[index >> 3] |= (u8)(1u << (index & 7));
    if (index - particle_highest > 0) {
        next = index + 1;
    } else {
        s32 byte = (index + 1) >> 3;
        next = byte << 3;
        while (next - 0x800 < 0) {
            const u8 bits = particle_allocated[byte];
            u32 bit = 1;
            s32 found = 0;
            while (1) {
                const u32 taken = bits & bit;
                bit <<= 1;
                if (taken == 0) { found = 1; break; }
                next++;
                if ((bit & 0xff) == 0) break;
            }
            if (found) break;
            byte++;
        }
    }
    particle_next_free = next;
    if (index > particle_highest) particle_highest = index;
    particle_count = particle_count + 1;
    particle = particle_table + 64 * index;
    __builtin_memset(particle, 0, 32);
    particle[0] = (u8)type;
    return particle;
}
/* The boot program's create_part (0x217a30, called by spawn_spark, 0x218888) is the same code with
   the executable's addresses, which the level build maps to the level's (the labels above). */
ALIAS(create_part, FUN_00217a30, FUN_L00_002678b8)

/* build_resident_visibility_list, level 0 copy at 0x250768 (ends 0x250a38): the frame's list of
   mobys to update. gp = 0x166c00. Scratchpad (local arrays here; the routine's use of it is
   private): 224 per-class {head, tail} pairs, a u16 index list, 112 group flags.
   For each moby in the table at [gp - 0x6c28] (0x15FFD8) until state 0xFF: a state with the sign
   bit set is skipped; flags & 2 is skipped. It is kept when +0x31 is set, when update_dist (+0x30)
   is 0xFF, or when update_dist^2 - |pos - camera|^2 has its sign bit clear (vmula/vmsuba, camera
   position at 0x166c80 + 0x140). A kept moby marks its group (+0x21) when it has one (and is then
   added with the group); otherwise it is appended to its class slot's list (+0x22, linked through
   +0x28) and, when flags & 0x1000, its index joins the index list. Then each marked group's member
   list (u16 indices from the table at 0x1ab840; the entry with bit 15 ends it) adds its live
   members the same way. The index list is written as moby pointers to 0x1aba00 with a 0 end, and
   the class lists are joined in slot order; the head is returned. */
extern u8 *level_moby_table __asm__("D_0015FF18") /* level copy D_L00_0015FFD8 */;
extern f32 level_camera_block[] __asm__("D_00186F40") /* level copy D_L00_00166C80 */;
extern u16 *level_moby_groups[] __asm__("D_001C7740") /* level copy D_L00_001AB840 */;
extern u8 *level_flagged_mobys[] __asm__("D_001C7900") /* level copy D_L00_001ABA00 */;
u8 *build_resident_visibility_list(void) __asm__("FUN_L00_00250768");
u8 *build_resident_visibility_list(void) {
    static u8 *heads[224][2];
    static u16 indices[0x200];
    static u8 group_marked[0x80];
    const f32 *camera = level_camera_block + 0x140 / 4;
    u8 *table = level_moby_table;
    s32 kept = 0, index;
    u8 *head = 0, *tail = 0;
    __builtin_memset(heads, 0, sizeof(heads));
    __builtin_memset(group_marked, 0, sizeof(group_marked));

#define APPEND(moby) do { \
        u8 **slot = heads[(moby)[0x22] % 224]; \
        indices[kept & 0x1ff] = (u16)index; \
        if (*(u16 *)((moby) + 0x34) & 0x1000) kept++; \
        if (slot[1]) *(u8 **)(slot[1] + 0x28) = (moby); else slot[0] = (moby); \
        slot[1] = (moby); \
    } while (0)

    for (index = 0;; index++) {
        u8 *moby = table + 0x100 * index;
        const s8 state = (s8)moby[0x20];
        const u16 flags = *(u16 *)(moby + 0x34);
        s8 group;
        if (state < 0) {
            if (state == -1) break;
            continue;
        }
        if (flags & 2) continue;
        if (moby[0x31] == 0 && moby[0x30] != 0xff) {
            const f32 dx = *(f32 *)(moby + 0x10) - camera[0];
            const f32 dy = *(f32 *)(moby + 0x14) - camera[1];
            const f32 dz = *(f32 *)(moby + 0x18) - camera[2];
            const f32 limit = (f32)moby[0x30];
            const f32 left = ((limit * limit - dx * dx) - dy * dy) - dz * dz;
            u32 bits;
            __builtin_memcpy(&bits, &left, 4);
            if (bits >> 31) continue;
        }
        group = (s8)moby[0x21];
        if (group >= 0) {
            group_marked[group & 0x7f] = 1;
            continue;
        }
        APPEND(moby);
    }
    for (s32 group = 0; group < 0x70; group++) {
        const u16 *member;
        s16 entry = 0;
        if (!group_marked[group]) continue;
        member = level_moby_groups[group];
        do {
            u8 *moby;
            entry = (s16)*member++;
            index = entry & 0x7fff;
            moby = table + 0x100 * index;
            if ((s8)moby[0x20] < 0) continue;
            APPEND(moby);
        } while (entry >= 0);
    }
#undef APPEND
    for (s32 i = 0; i < kept; i++) level_flagged_mobys[i] = table + 0x100 * indices[i];
    level_flagged_mobys[kept] = 0;
    for (s32 slot = 0; slot < 224; slot++) {
        if (!heads[slot][0]) continue;
        if (!head) head = heads[slot][0];
        else *(u8 **)(tail + 0x28) = heads[slot][0];
        tail = heads[slot][1];
        *(u8 **)(tail + 0x28) = 0;
    }
    return head;
}

/* update_all_mobys, level 0 0x264848: head = build_resident_visibility_list(), stored at
   [gp - 0x6c1c] (0x15FFE4); for each moby (next at +0x28) whose state has the sign bit clear:
   advance_moby_animation (0x250480) unless flags & 0x40, the class update (+0x74) if any,
   update_entity (0x250df8) unless flags & 4 (re-read after the update). */
extern u8 *level_update_list_head __asm__("D_0015FF24") /* level copy D_L00_0015FFE4 */;
extern void level_advance_moby_animation(void *moby) __asm__("FUN_L00_00250480");
extern void level_update_entity(void *moby) __asm__("FUN_L00_00250df8");
void update_all_mobys(void) __asm__("FUN_L00_00264848");
void update_all_mobys(void) {
    u8 *moby = build_resident_visibility_list();
    level_update_list_head = moby;
    for (; moby != 0; moby = *(u8 **)(moby + 0x28)) {
        void (*update)(void *);
        if ((s8)moby[0x20] < 0) continue;
        if (!(*(u16 *)(moby + 0x34) & 0x40)) level_advance_moby_animation(moby);
        update = *(void (**)(void *))(moby + 0x74);
        if (update) update(moby);
        if (!(*(volatile u16 *)(moby + 0x34) & 4)) level_update_entity(moby);
    }
}

/* 0x1fa328 multiply_matrix3x3(out, a, b): rows of a in vf4-vf6, rows of b in vf1-vf3 (raw fields:
   fs = a's row, ft = b's row, broadcast x/y/z); out row i = (a0 * b[i].x + a1 * b[i].y) + a2 * b[i].z
   over xyzw. Three rows; out's fourth row is not written. */
void multiply_matrix3x3(void *out, void *a_in, void *b_in) __asm__("FUN_001fa328");
void multiply_matrix3x3(void *out, void *a_in, void *b_in) {
    const u8 *a = a_in, *b = b_in;
    Vec4 r[3];
    const Vec4 a0 = load_vec(a), a1 = load_vec(a + 16), a2 = load_vec(a + 32);
    for (s32 i = 0; i < 3; i++) {
        const Vec4 bi = load_vec(b + 16 * i);
        r[i].x = (a0.x * bi.x + a1.x * bi.y) + a2.x * bi.z;
        r[i].y = (a0.y * bi.x + a1.y * bi.y) + a2.y * bi.z;
        r[i].z = (a0.z * bi.x + a1.z * bi.y) + a2.z * bi.z;
        r[i].w = (a0.w * bi.x + a1.w * bi.y) + a2.w * bi.z;
    }
    for (s32 i = 0; i < 3; i++) store_vec((u8 *)out + 16 * i, r[i]);
}
ALIAS(multiply_matrix3x3, func_001FA328, FUN_001fa328)

/* 0x1f9c48 set_vector_length_xy(out, in, length): s = x*x + y*y (vaddy.x); vrsqrt Q = length /
   sqrt(s); if the bits of s are zero, xy = 0, else xy *= Q. z and w keep in's. */
void set_vector_length_xy(void *out, void *in, f32 length) __asm__("FUN_001f9c48");
void set_vector_length_xy(void *out, void *in, f32 length) {
    Vec4 v = load_vec(in);
    const f32 s = v.x * v.x + v.y * v.y;
    u32 bits;
    __builtin_memcpy(&bits, &s, 4);
    if (bits == 0) {
        v.x = v.y = 0.0f;
    } else {
        const f32 q = length / __builtin_sqrtf(s);
        v.x *= q; v.y *= q;
    }
    store_vec(out, v);
}

/* Level 0 copies of boot-program helpers. Each listed copy's instruction words equal the boot
   function's, jumps aside. (0x1ff8b0, fast_arc_tan's copy, is aliased in browser/level-math.c.) */
ALIAS(fast_dec_timer_s16, FUN_L00_001fef98, FUN_001f9770)
ALIAS(scale_game_frames, FUN_L00_001fef20, FUN_001f96f8)
ALIAS(fast_vec_scale, FUN_L00_001ff300, FUN_001f9a68)
ALIAS(fast_cos, FUN_L00_001ff7e8, FUN_001f9dc8)
ALIAS(fast_sin, FUN_L00_001ff800, FUN_001f9de0)
ALIAS(fast_vec_length, FUN_L00_001ff3d8, FUN_001f9af0)
ALIAS(fast_vec_add, FUN_L00_001ff278, FUN_001f9a10)
ALIAS(set_vector_length_xy, FUN_L00_001ff550, FUN_001f9c48)

/* Level 0x25c338 / 0x25c368: 0 for a null moby or one without flags & 0x20, else word 0 (resp.
   word 4) of its pvars (+0x78). */
extern f32 fast_arc_tan(f32 x, f32 y) __asm__("FUN_001f9e90");
static u8 *moby_pvar_word(u8 *moby, s32 offset) {
    if (!moby || !(*(u16 *)(moby + 0x34) & 0x20)) return 0;
    return *(u8 **)(*(u8 **)(moby + 0x78) + offset);
}
u8 *moby_health_block(u8 *moby) __asm__("FUN_L00_0025c338");
u8 *moby_health_block(u8 *moby) { return moby_pvar_word(moby, 0); }
u8 *moby_hit_effects_block(u8 *moby) __asm__("FUN_L00_0025c368");
u8 *moby_hit_effects_block(u8 *moby) { return moby_pvar_word(moby, 0x10); }

/* hit_reaction, level 0 0x25a478 (352 instructions, compiler output; three jump tables at
   0x1e8f40, 0x1e8f60 and 0x1e8f90 in level data). Arguments: the moby, its hit record (or 0),
   its health block (0: from moby_health_block), a mask of reactions it ignores, and out-pointers
   for a status, the damage and the attacker's class; the last argument selects the table row
   (4: the health block's byte 8). Returns the reaction from the byte table at 0x1b0180.
   Hit record: +0x10 knockback vector, +0x1c a marker (5627.95: knockback not yet applied),
   +0x20 attacker moby, +0x28 hit type, +0x29 strength, +0x2a attacker class, +0x2c damage.
   Health block: +0x08 kind, +0x14/+0x16 last attacker class, +0x1c cooldown timer (s16). */
extern f32 hit_default_damage[] __asm__("D_0015FFC8") /* level copy D_L00_00160080 */;       /* gp - 0x6b80 */
extern f32 hit_knockback_scale[] __asm__("D_001CC0C8") /* level copy D_L00_001B01C8 */;      /* [row][hit type] */
extern u8 hit_reaction_by_kind[] __asm__("D_001CC080") /* level copy D_L00_001B0180 */;      /* [reaction * 4 + kind] */
extern u8 hit_hero_base[] __asm__("D_0013F350");                 /* hero; +0x80 position */
extern u8 hit_cooldown_switch[] __asm__("D_0013E533");
s32 hit_reaction(u8 *moby, u8 *hit, u8 *health, s32 ignore, s32 *status, f32 *damage_out,
                 s32 *attacker_class_out, s32 row) __asm__("FUN_L00_0025a478");
s32 hit_reaction(u8 *moby, u8 *hit, u8 *health, s32 ignore, s32 *status, f32 *damage_out,
                 s32 *attacker_class_out, s32 row) {
    const f32 marker = 5627.9497f; /* 0x45afdf66 */
    s32 reaction, result, attacker = 0;
    *status = 0;
    if (attacker_class_out) *attacker_class_out = -1;
    if (hit && *(u8 **)(hit + 0x20)) {
        const s16 source = *(s16 *)(*(u8 **)(hit + 0x20) + 0xa6);
        if ((source == 0x131 || source == 0xa8) && health) *(f32 *)(hit + 0x2c) = hit_default_damage[health[8]];
    }
    if (!moby) {
        if (health) {
            *(s16 *)(health + 0x1c) = 0;
            *(s16 *)(health + 0x14) = -1;
            *(s16 *)(health + 0x16) = -1;
        }
        *status = 0;
        return 0;
    }
    if (!health) {
        health = moby_health_block(moby);
        if (!health) { *status = 0; return 0; }
    }
    if (row == 4) row = health[8];
    if (!hit) {
        fast_dec_timer_s16((s16 *)(health + 0x1c));
        *(s16 *)(health + 0x16) = -1;
        *status = 1;
        if (damage_out) *damage_out = 0.0f;
        return 0xb;
    }
    if (*(f32 *)(hit + 0x1c) == marker) {
        u8 *knockback = hit + 0x10;
        fast_vec_scale(knockback, knockback, hit_knockback_scale[row * 8 + hit[0x28]]);
        attacker = *(u16 *)(hit + 0x2a);
        *(f32 *)(hit + 0x1c) = marker;
        if (attacker == 0x47) { /* the hero: push away from it, mostly level */
            f32 away[4] __attribute__((aligned(16)));
            f32 sum[4] __attribute__((aligned(16)));
            const f32 *hero = (const f32 *)(hit_hero_base + 0x80);
            away[0] = fast_cos(fast_arc_tan(*(f32 *)(moby + 0x10) - hero[0], *(f32 *)(moby + 0x14) - hero[1]));
            away[1] = fast_sin(fast_arc_tan(*(f32 *)(moby + 0x10) - hero[0], *(f32 *)(moby + 0x14) - hero[1]));
            away[2] = 1.0f;
            away[3] = 0.0f; /* retail leaves this stack word unwritten */
            fast_vec_normalize(away, away, fast_vec_length(knockback) * 0.7f);
            fast_vec_scale(knockback, knockback, 0.3f);
            fast_vec_add(sum, knockback, away);
            __builtin_memcpy(knockback, sum, 16);
            set_vector_length_xy(knockback, knockback, 1.0f);
        }
    } else {
        u8 *source = *(u8 **)(hit + 0x20);
        attacker = source ? *(s16 *)(source + 0xa6) : 0x47;
        hit[0x28] = 8;
    }
    switch (hit[0x28]) {
    case 0: reaction = (u32)(*(u16 *)(hit + 0x2a) - 1) < 2 ? 0 : 1; *status = 2; break;
    case 1:
        *status = 2;
        reaction = hit[0x29] < 2 ? 0xb : hit[0x29] == 2 ? 5 : 9;
        break;
    case 2: reaction = hit[0x29] < 2 ? 8 : 0xd; *status = 2; break;
    case 3:
        *status = 2;
        if (hit[0x29] < 2) reaction = 0xa;
        else if (hit[0x29] < 3) reaction = 0xf;
        else { reaction = 0x10; *(s16 *)(health + 0x14) = -1; *(s16 *)(health + 0x1c) = 0; }
        break;
    case 4: reaction = 0xe; *status = 2; break;
    case 5:
        if (hit[0x29] < 2) { reaction = 4; *status = 3; } else { reaction = 7; *status = 4; }
        break;
    case 7: reaction = hit[0x29] < 2 ? 0xc : 0x11; *status = 2; break;
    default: reaction = 1; *status = 2; break; /* 6 and >= 8 */
    }
    if (attacker_class_out) *attacker_class_out = attacker;
    result = hit_reaction_by_kind[health[8] + reaction * 4];
    if (*(s16 *)(health + 0x14) != attacker) *(s16 *)(health + 0x1c) = 0;
    if (fast_dec_timer_s16((s16 *)(health + 0x1c))) {
        s32 frames = 0x1e;
        switch (reaction) {
        case 0: frames = 0x25; break;
        case 1: frames = 0xf; break;
        case 4: case 11: frames = 0x3c; break;
        case 7: frames = hit_cooldown_switch[0] ? 0x2d : 0x3c; break;
        default: break;
        }
        *(s16 *)(health + 0x1c) = (s16)scale_game_frames(frames);
    } else {
        s32 bits = -1, cleared = 0;
        switch (reaction) {
        case 0: case 1: bits = 0x10; cleared = 0x20; break;
        case 4: bits = 0x1; cleared = 0x2; break;
        case 7: bits = 0x4; cleared = 0x8; break;
        case 11: result = 0xc; break;
        default: *(f32 *)(hit + 0x2c) = 0.0f; result = 0xd; break;
        }
        if (bits >= 0) {
            if (!(ignore & bits)) result = 0xb;
            if (!(ignore & cleared)) *status = 1;
            *(f32 *)(hit + 0x2c) = 0.0f;
        }
    }
    *(s16 *)(health + 0x16) = (s16)attacker;
    *(s16 *)(health + 0x14) = (s16)attacker;
    if (damage_out) *damage_out = *(f32 *)(hit + 0x2c);
    {
        u8 *effects = moby_hit_effects_block(moby);
        if (effects) effects[0x2e] = reaction == 4;
    }
    return result;
}

/* 0x20ede8 (553 instructions), called by blend_moby_animation as (moby, render slot | 0x300):
   bakes the moby's current blended pose into an ordinary animation frame at
   0x1aabc0 + 0x800 * slot (debug_font_buffer), which update_moby_animation_state uses as the
   frame of a moby whose sequence is 0xFF, so the next sequence blends out of it.
   The pose is the local stage of 0x2109b8 (rnc_moby_local_pose). Its parked-pose and
   manipulator pass (taken at 0x20f460 when +0x60 or +0x64 is set, path 0x20f810) is ASSUMED to be
   that of 0x2109b8; it is not decoded.
   Encoding (0x20f468-0x20f650; MMI words decoded): per joint, the quaternion as vftoi15, clamped
   to int16 (pmaxw/pminw) and packed (ppach), 8 bytes; a scale record when the scale, as
   vftoi15 >> 3 (x4096) clamped to 0..0xFFFF, is not 0x1000 in x, y and z: { x, y, z, joint byte,
   ((flag word & 0x80) ^ 0x80) }; a translation record when its vftoi0 x, y, z differ from the
   bind translation's (class joint record): { x, y, z, joint as u16 }. Header: 8 zero bytes
   (rate 0, source frame 0) then +6 quadwords ((data + 8) >> 4), +8 rotation bytes, +0xA scale
   count, +0xC rotation + scale bytes, +0xE translation count; the records follow the rotations,
   then an 8-byte 1. (header quadwords + 1) quadwords are copied out (DMA channel 8). */
extern u8 parked_pose_frames[] __asm__("D_001AABC0");
static s32 ee_ftoi(f32 x) {
    if (x != x) return 0;
    if (x >= 2147483648.0f) return 0x7fffffff;
    if (x <= -2147483648.0f) return (s32)0x80000000;
    return (s32)x;
}
static s16 clamp16(s32 v) { return (s16)(v < -0x8000 ? -0x8000 : v > 0x7fff ? 0x7fff : v); }
void park_moby_pose(u8 *moby, s32 slot) __asm__("FUN_0020ede8");
void park_moby_pose(u8 *moby, s32 slot) {
    static u8 frame[0x800] __attribute__((aligned(16)));
    const u8 *pclass = *(u8 **)(moby + 0x24);
    const u8 *bind = *(const u8 **)(pclass + 0x18);
    const s32 joints = pclass[8];
    u8 scales[0x400], translations[0x400];
    s32 scale_bytes = 0, translation_bytes = 0, rotation_bytes = joints * 8, quadwords, at;
    if (joints == 0) return;
    rnc_moby_local_pose(moby);
    __builtin_memset(frame, 0, 16);
    for (s32 j = 0; j < joints; j++) {
        const f32 *s = rnc_moby_pose_slot(j);
        s16 rec[4];
        s32 sx, sy, sz;
        for (s32 k = 0; k < 4; k++) rec[k] = clamp16(ee_ftoi(s[k] * 32768.0f));
        __builtin_memcpy(frame + 16 + 8 * j, rec, 8);
        sx = (s32)((u32)ee_ftoi(s[4] * 32768.0f) >> 3);
        sy = (s32)((u32)ee_ftoi(s[5] * 32768.0f) >> 3);
        sz = (s32)((u32)ee_ftoi(s[6] * 32768.0f) >> 3);
        sx = sx < 0 ? 0 : sx > 0xffff ? 0xffff : sx;
        sy = sy < 0 ? 0 : sy > 0xffff ? 0xffff : sy;
        sz = sz < 0 ? 0 : sz > 0xffff ? 0xffff : sz;
        if (!(sx == 0x1000 && sy == 0x1000 && sz == 0x1000) && scale_bytes < 0x3f8) {
            u8 *r = scales + scale_bytes;
            u32 flag;
            __builtin_memcpy(&flag, s + 7, 4);
            *(u16 *)(r + 0) = (u16)sx; *(u16 *)(r + 2) = (u16)sy; *(u16 *)(r + 4) = (u16)sz;
            r[6] = (u8)j; r[7] = (u8)((flag & 0x80) ^ 0x80);
            scale_bytes += 8;
        }
        {
            const f32 *b = (const f32 *)(bind + 16 * j);
            const s16 tx = (s16)ee_ftoi(s[8]), ty = (s16)ee_ftoi(s[9]), tz = (s16)ee_ftoi(s[10]);
            if ((tx != (s16)ee_ftoi(b[0]) || ty != (s16)ee_ftoi(b[1]) || tz != (s16)ee_ftoi(b[2])) && translation_bytes < 0x3f8) {
                u8 *r = translations + translation_bytes;
                *(s16 *)(r + 0) = tx; *(s16 *)(r + 2) = ty; *(s16 *)(r + 4) = tz;
                *(u16 *)(r + 6) = (u16)j;
                translation_bytes += 8;
            }
        }
    }
    at = 16 + rotation_bytes;
    __builtin_memcpy(frame + at, scales, scale_bytes); at += scale_bytes;
    __builtin_memcpy(frame + at, translations, translation_bytes); at += translation_bytes;
    *(u64 *)(frame + at) = 1;
    quadwords = (rotation_bytes + scale_bytes + translation_bytes + 8) >> 4;
    *(u16 *)(frame + 6) = (u16)quadwords;
    *(u16 *)(frame + 8) = (u16)rotation_bytes;
    *(u16 *)(frame + 0xa) = (u16)(scale_bytes >> 3);
    *(u16 *)(frame + 0xc) = (u16)(rotation_bytes + scale_bytes);
    *(u16 *)(frame + 0xe) = (u16)(translation_bytes >> 3);
    __builtin_memcpy(parked_pose_frames + 0x800 * (slot & 0xff), frame, 16 * (quadwords + 1) <= 0x800 ? 16 * (quadwords + 1) : 0x800);
}
ALIAS(park_moby_pose, func_0020EDE8, FUN_0020ede8)

/* fast_bsphere_check(sphere, range), boot 0x1fa728 and its level 0 copy at 0x2002e0 (the same 64
   instructions; the copy's view block is at 0x16ccc0 instead of 0x18cf80). Returns -1 outside the
   view, 0 crossing its edge, 1 inside. View block T (0x80 bytes): rows 0-2 a rotation, row 3 the
   camera position with w a scale, rows 4-7 per-axis factors. Decoded with the raw VU fields:
     p = sphere * T3.w (xyzw); v = (T0 * d.x + T1 * d.y) + T2 * d.z with d = p.xyz - T3.xyz;
     w = p.w; a = 0 - (w + v.z); b = range * T3.w - (v.z - w)
     -1 if b's sign bit is set or a's is clear;
     e = T4.xy * v.z - (|v.xy| - T6.xy * w); f = (T4.xy * v.z) * T5.xy - (|v.xy| + (T6.xy * w) * T6.z)
     g = ((v.z + w) - T7.x with its sign bit flipped, (v.z - w) - T7.y)
     -1 if e.y or e.x has its sign bit set; 0 if (f | g).y or (f | g).x does; else 1.
   Recovered level code names the boot label; the level program's call (e.g. 0x2d71d8,
   jal 0x2002e0) goes to the copy, and 0x18cf80 is level data once the level is loaded, so both
   labels use the level's view block here. */
extern f32 level_view_block[] __asm__("D_0018CF80") /* level copy D_L00_0016CCC0 */;
static u32 fbits(f32 f) { u32 b; __builtin_memcpy(&b, &f, 4); return b; }
static s32 bsphere_check(const f32 *sphere, f32 range, const f32 *T) {
    const f32 s = T[15];
    const f32 px = sphere[0] * s, py = sphere[1] * s, pz = sphere[2] * s, w = sphere[3] * s;
    const f32 dx = px - T[12], dy = py - T[13], dz = pz - T[14];
    const f32 vx = (T[0] * dx + T[4] * dy) + T[8] * dz;
    const f32 vy = (T[1] * dx + T[5] * dy) + T[9] * dz;
    const f32 vz = (T[2] * dx + T[6] * dy) + T[10] * dz;
    const f32 a = 0.0f - (w + vz), b = range * s - (vz + -w);
    const f32 ax = __builtin_fabsf(vx), ay = __builtin_fabsf(vy);
    f32 e[2], f[2];
    u32 g[2];
    if ((fbits(b) >> 31) || !(fbits(a) >> 31)) return -1;
    e[0] = T[16] * vz - (ax - T[24] * w);
    e[1] = T[17] * vz - (ay - T[25] * w);
    f[0] = (T[16] * vz) * T[20] - (ax + (T[24] * w) * T[26]);
    f[1] = (T[17] * vz) * T[21] - (ay + (T[25] * w) * T[26]);
    g[0] = fbits((w + vz) - T[28]) ^ 0x80000000u;
    g[1] = fbits((vz + -w) - T[29]);
    if ((fbits(e[1]) >> 31) || (fbits(e[0]) >> 31)) return -1;
    if (((fbits(f[1]) | g[1]) >> 31) || ((fbits(f[0]) | g[0]) >> 31)) return 0;
    return 1;
}
s32 fast_bsphere_check(void *sphere, f32 range) __asm__("FUN_001fa728");
s32 fast_bsphere_check(void *sphere, f32 range) { return bsphere_check(sphere, range, level_view_block); }
ALIAS(fast_bsphere_check, FUN_L00_002002e0, FUN_001fa728)

/* 0x1fa6e0 fast_tween_color(a, b, t): pextlb/pextlh widen each byte of a and b (unsigned) to a
   32-bit lane; vitof0; vf1.w = 1 - t; ACC = a * (1 - t); result = ACC + b * t; vftoi0 truncates;
   ppach/ppacb keep each lane's low byte. */
u32 fast_tween_color(u32 a, u32 b, f32 t) __asm__("FUN_001fa6e0");
u32 fast_tween_color(u32 a, u32 b, f32 t) {
    const f32 keep = 1.0f - t;
    u32 out = 0;
    for (s32 k = 0; k < 4; k++) {
        const f32 x = (f32)((a >> (8 * k)) & 0xff) * keep + (f32)((b >> (8 * k)) & 0xff) * t;
        out |= ((u32)ee_ftoi(x) & 0xff) << (8 * k);
    }
    return out;
}
ALIAS(fast_tween_color, func_001FA6E0, FUN_001fa6e0)

/* set_moby_color(moby, r, g, b), level 0 0x2502f0: the 64-bit word at moby + 0x38 keeps its low
   32 bits and gets (r << 32) | (g << 40) | (b << 48) ORed above them (dsll32 by 0, 8, 16; no
   masking). */
void set_moby_color(u8 *moby, s32 r, s32 g, s32 b) __asm__("FUN_L00_002502f0");
void set_moby_color(u8 *moby, s32 r, s32 g, s32 b) {
    u64 word = *(u64 *)(moby + 0x38) & 0xffffffffull;
    word |= (u64)(s64)r << 32;
    word |= (u64)(s64)g << 40;
    word |= (u64)(s64)b << 48;
    *(u64 *)(moby + 0x38) = word;
}

/* Level 0 0x1ff408 is a word-for-word copy of vector_length_xy (0x1f9b20, level-math.c). */
f32 vector_length_xy(void *v) __asm__("FUN_001f9b20");
f32 level_vector_length_xy(void *v) __asm__("FUN_L00_001ff408");
f32 level_vector_length_xy(void *v) { return vector_length_xy(v); }

/* 0x1f9fc8 set_identity_matrix: vf0 = (0, 0, 0, 1); rows (1,0,0,0), (0,1,0,0), vmr32 of vf0
   (0,0,1,0) and vf0 itself (0,0,0,1), stored as four quadwords. */
void set_identity_matrix(void *out) __asm__("FUN_001f9fc8");
void set_identity_matrix(void *out) {
    static const f32 identity[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    __builtin_memcpy(out, identity, sizeof(identity));
}

/* Level 0 0x1fff28 build_axis_rotation_quaternion(out, axis, angle); returns through t7, which is
   why level-copies.py (looking for jr ra) does not see it. n = -fast_sin(angle),
   c2 = 2 * fast_cos(angle) (the level copies of 0x1f9de0 / 0x1f9dc8); s = sqrt(|c2| + 2).
   c2 >= 0 (sign bit clear): q = (n / s, 0, 0, s / 2); otherwise q = (s / 2, 0, 0, n / s).
   axis 1 moves x into y, axis 2 into z (vaddx.y / vaddx.z, then x = 0); one sqc2 stores q. */
void build_axis_rotation_quaternion(void *out, s32 axis, f32 angle) __asm__("FUN_L00_001fff28");
void build_axis_rotation_quaternion(void *out, s32 axis, f32 angle) {
    const f32 n = -fast_sin(angle);
    const f32 c2 = fast_cos(angle) + fast_cos(angle);
    const f32 s = __builtin_sqrtf(__builtin_fabsf(c2) + 2.0f);
    f32 q[4] = { 0, 0, 0, 0 };
    u32 bits;
    __builtin_memcpy(&bits, &c2, 4);
    if (!(bits & 0x80000000u)) { q[3] = 0.5f * s; q[0] = n / s; }
    else { q[0] = 0.5f * s; q[3] = n / s; }
    if (axis == 1) { q[1] = q[0]; q[0] = 0; }
    else if (axis != 0) { q[2] = q[0]; q[0] = 0; }
    __builtin_memcpy(out, q, sizeof(q));
}

/* Level 0 0x2483c8 unpack_menu_image(dst, key, source, offsets): 4-bit run-length image decode
   for ensure_menu_cache_entry. Image `key` spans source + offsets[key - 1] .. source +
   offsets[key] (key 0 starts at source + 0x200) as 12-bit records, three nibbles each
   (count = bytes * 2 / 3). Record i starts at nibble 3i: on an even nibble the value is byte0 & 15
   and the run (byte1 << 4 | byte0 >> 4) & 0xff; on an odd nibble the value is byte0 >> 4 and the
   run byte1. A run of 0 means 256. Pixels are written two per byte (low nibble first); an odd
   run leaves the last byte with only its low nibble, and the next record ORs its first pixel
   into the high nibble. */
void unpack_menu_image(u8 *dst, s32 key, u8 *source, s16 *offsets) __asm__("FUN_L00_002483c8");
void unpack_menu_image(u8 *dst, s32 key, u8 *source, s16 *offsets) {
    const u8 *start = key != 0 ? source + offsets[key - 1] : source + 0x200;
    const s32 count = (s32)((source + offsets[key]) - start) * 2 / 3;
    s32 pending = 0;
    for (s32 i = 0; i < count; i++) {
        const s32 nibble = i * 3;
        const u8 *at = start + (nibble >> 1);
        u32 value, run;
        if (nibble & 1) { value = at[0] >> 4; run = at[1]; }
        else { value = at[0] & 15; run = ((u32)(at[1] << 4) | (at[0] >> 4)) & 0xff; }
        s32 left = run ? (s32)run : 0x100;
        const u8 pair = (u8)(value | value << 4);
        if (pending) { pending = 0; left -= 1; *dst = (u8)(*dst | value << 4); dst++; }
        if (left == 0) continue;
        do { *dst++ = pair; left -= 2; } while (left > 0);
        if (left != 0) { dst--; pending = 1; *dst = (u8)value; }
    }
}

/* Level 0 moby lighting (hand-written EE/VU0/MMI assembly in evaluate_moby_animation_pose.c).
   Tables are those load_level_gameplay fills: light regions (D_L00_0017ECC0, xyz centre and w a
   squared radius, D_L00_0015FBC4 of them), their 0x80-byte records (D_L00_0017EEC0: a 4x4 matrix,
   colours at +0x40 / +0x44, bytes at +0x48 / +0x4C, flags at +0x50), the point-light grid
   (D_L00_0015FBC0: 64 x 64 cells of 16 units, each a byte offset to { count, indices }) and the
   point lights (D_L00_0015FB80, 32 bytes each, w the squared radius, colour at +0x10). Both
   tests are on x and y only (vaddy.x). */
extern s32 light_region_count __asm__("D_0015FB44") /* level copy D_L00_0015FBC4 */;
extern u8 light_region_spheres[] __asm__("D_0019ABC0") /* level copy D_L00_0017ECC0 */;
extern u8 light_region_records[] __asm__("D_0019ADC0") /* level copy D_L00_0017EEC0 */;
extern u8 *point_light_grid __asm__("D_0015FB40") /* level copy D_L00_0015FBC0 */;
extern u8 *point_lights __asm__("D_0015FB00") /* level copy D_L00_0015FB80 */;

static f32 word_float(const void *at) { f32 f; __builtin_memcpy(&f, at, 4); return f; }

/* 0x256de0: the first region (index 0 through count inclusive: the loop tests the pointer before
   advancing it) whose xy circle holds pos; its matrix takes (pos, 1), and a result outside
   [-1, 1] on x, y or z (vclipw against vf0.w) finds nothing. *t = (x + 1) / 2, *index = i. */
static s32 find_light_region(const f32 *pos, f32 *t, s32 *index) {
    for (s32 i = 0; i <= light_region_count; i++) {
        const u8 *s = light_region_spheres + 16 * i;
        const f32 dx = pos[0] - word_float(s), dy = pos[1] - word_float(s + 4);
        if (!(dx * dx + dy * dy - word_float(s + 12) < 0.0f)) continue;
        const u8 *m = light_region_records + 0x80 * i;
        f32 out[3];
        for (s32 k = 0; k < 3; k++) {
            out[k] = word_float(m + 4 * k) * pos[0] + word_float(m + 16 + 4 * k) * pos[1] +
                     word_float(m + 32 + 4 * k) * pos[2] + word_float(m + 48 + 4 * k);
            if (out[k] > 1.0f || out[k] < -1.0f) return 0;
        }
        *index = i;
        *t = 0.5f * (out[0] + 1.0f);
        return 1;
    }
    return 0;
}

/* 0x256f14: the grid cell of (x >> 4, y >> 4) (cvt.w.s, then a logical shift), then the first of
   its point lights whose xy circle holds pos; *distance2 = the squared xy distance. */
static const u8 *find_point_light(const f32 *pos, f32 *distance2) {
    const u32 cx = (u32)ee_ftoi(pos[0]) >> 4, cy = (u32)ee_ftoi(pos[1]) >> 4;
    const s32 offset = *(const s32 *)(point_light_grid + 4 * (cy * 64 + cx));
    if (offset == 0) return 0;
    const u8 *list = point_light_grid + offset;
    s32 left = *(const s32 *)list - 1;
    for (const s32 *entry = (const s32 *)(list + 4);; entry++) {
        const u8 *light = point_lights + 32 * *entry;
        const f32 dx = pos[0] - word_float(light), dy = pos[1] - word_float(light + 4);
        const f32 d2 = dx * dx + dy * dy;
        if (d2 - word_float(light + 12) < 0.0f) { *distance2 = d2; return light; }
        if (left == 0) return 0;
        left--;
    }
}

/* Per byte lane of the MMI sequences below: pmulth's signed 16-bit products, then psrlh 8 and
   ppach / ppacb keep bits 8-15 of each product. */
static u32 lane_scale(u32 colour, s32 scale, s32 k) {
    return ((u32)((s32)(s16)((colour >> (8 * k)) & 0xff) * (s32)(s16)scale) >> 8) & 0xff;
}

/* 0x257024 update_moby_lighting(moby): moby + 0x10 is the position. In a region with flag bit 0,
   w = (int)(255 * t): moby + 0x80 = per byte lane, bits 8-15 of c0 * (255 - w) plus bits 8-15 of
   c1 * w with unsigned byte saturation (paddub adds byte lanes, so no carry from bits 0-7), and
   the low word at moby + 0x38 = b48 | b4C << 8 | w << 16. Then moby + 0x3C = moby + 0x80, plus
   (saturating per byte) a point light's colour scaled by 255 - (int)(255 * d2 / r2). */
void update_moby_lighting(u8 *moby) __asm__("FUN_L00_00257024");
void update_moby_lighting(u8 *moby) {
    const f32 *pos = (const f32 *)(moby + 0x10);
    f32 t, d2;
    s32 index;
    if (find_light_region(pos, &t, &index)) {
        const u8 *record = light_region_records + 0x80 * index;
        if (*(const u32 *)(record + 0x50) & 1) {
            const s32 w = ee_ftoi(255.0f * t);
            const u32 c0 = *(const u32 *)(record + 0x40), c1 = *(const u32 *)(record + 0x44);
            u32 out = 0;
            for (s32 k = 0; k < 4; k++) {
                const u32 sum = lane_scale(c0, 255 - w, k) + lane_scale(c1, w, k);
                out |= (sum > 255 ? 255 : sum) << (8 * k);
            }
            *(u32 *)(moby + 0x80) = out;
            *(u32 *)(moby + 0x38) = *(const u32 *)(record + 0x48) | *(const u32 *)(record + 0x4C) << 8 |
                                    (u32)w << 16;
        }
    }
    const u8 *light = find_point_light(pos, &d2);
    const u32 base = *(const u32 *)(moby + 0x80);
    if (!light) { *(u32 *)(moby + 0x3C) = base; return; }
    const s32 scale = 255 - ee_ftoi(d2 / word_float(light + 12) * 255.0f);
    const u32 colour = *(const u32 *)(light + 0x10);
    u32 out = 0;
    for (s32 k = 0; k < 4; k++) {
        const u32 sum = lane_scale(colour, scale, k) + ((base >> (8 * k)) & 0xff);
        out |= (sum > 255 ? 255 : sum) << (8 * k);
    }
    *(u32 *)(moby + 0x3C) = out;
}

/* 0x20d510 get_moby_light_direction(moby, out): the word at moby + 0x38 holds light indices a
   (byte 0) and b (byte 1) and a blend w (byte 2), as update_moby_lighting stores them. Entries are
   64 bytes at D_0019BDC0, the vector at +0x10. w = 0: out = A. Otherwise f = (w << 4) / 4096
   (vitof12), out.xyz = A.xyz * (1 - f) + B.xyz * f and out.w = A.w. */
extern u8 moby_light_table[] __asm__("D_0019BDC0");
void get_moby_light_direction(u8 *moby, f32 *out) __asm__("FUN_0020d510");
void get_moby_light_direction(u8 *moby, f32 *out) {
    const u32 word = *(const u32 *)(moby + 0x38);
    const f32 *a = (const f32 *)(moby_light_table + 64 * (word & 0xff) + 0x10);
    const f32 *b = (const f32 *)(moby_light_table + 64 * ((word >> 8) & 0xff) + 0x10);
    const s32 w = (word >> 16) & 0xff;
    f32 r[4] = { a[0], a[1], a[2], a[3] };
    if (w > 0) {
        const f32 f = (f32)(w << 4) / 4096.0f, keep = 1.0f - f;
        for (s32 k = 0; k < 3; k++) r[k] = a[k] * keep + b[k] * f;
    }
    __builtin_memcpy(out, r, sizeof(r));
}

/* update_parts, level 0 copy at 0x267c08 (gp = 0x166c00); the boot copy at 0x217b88 is the
   same code on the boot globals (gp - 0x6b54 ...). update_cutscene_playback.c names the boot
   address; the level program's jal goes to 0x267c08 (level-copies.json, level_calls_00). Particles are 64 bytes from the table at [gp - 0x6a94] (create_part's) up to and
   including index [gp - 0x6a8c] (the highest in use). One whose byte 1 is negative is skipped;
   otherwise handler D_L00_001B1E80[(s8)byte 0] (register_part_types fills it) runs on it. The
   cursor and the end are kept at gp - 0x6a70 / - 0x6a6c and re-read after each call, so a handler
   may move them. */
extern u8 *particle_cursor[2] __asm__("D_001600D0") /* level copy D_L00_00160190 */;
extern void (*particle_handlers[])(u8 *) __asm__("D_001CDD80") /* level copy D_L00_001B1E80 */;
void update_parts(void) __asm__("FUN_L00_00267c08");
void update_parts(void) {
    u8 *at = particle_table;
    u8 *end = particle_table + 64 * (particle_highest + 1);
    particle_cursor[1] = end;
    while (at != end) {
        const s8 live = (s8)at[1], type = (s8)at[0];
        at += 0x40;
        if (live < 0) continue;
        particle_cursor[0] = at;
        particle_handlers[type](at - 0x40);
        at = particle_cursor[0];
        end = particle_cursor[1];
    }
}

/* free_effect_particle, level 0 0x267a08 (create_part's globals): index = (p - table) >> 6;
   count - 1; if p[1] & 0x40 its bit in the second bit set (0x1b1880 + 0x100) is cleared; its bit
   in the allocation set is cleared. When it was the highest index, the highest becomes the next
   lower allocated index (-1 when none), scanning bits downward. p[1] = 0x80 (free) and
   next_free = min(next_free, index) (pminw). */
extern u8 particle_allocated_both[0x200] __asm__("D_001CD780") /* level copy D_L00_001B1880 */;
void free_effect_particle(u8 *p) __asm__("FUN_L00_00267a08");
void free_effect_particle(u8 *p) {
    const s32 index = (s32)((u32)(p - particle_table) >> 6);
    const u8 keep = (u8)(0xff ^ (1u << (index & 7)));
    particle_count = particle_count - 1;
    if (p[1] & 0x40) particle_allocated_both[0x100 + (index >> 3)] &= keep;
    particle_allocated_both[index >> 3] &= keep;
    if (index == particle_highest) {
        s32 highest = index - 1;
        while (highest >= 0 && !(particle_allocated_both[highest >> 3] & (1u << (highest & 7)))) highest--;
        particle_highest = highest;
    }
    p[1] = 0x80;
    if (index < particle_next_free) particle_next_free = index;
}

/* 0x1f9c90 clamp_vector_length(out, in, max): len = sqrt(x*x + y*y + z*z). If len's bits are 0
   or max - len is positive (its float bits as a signed integer > 0), nothing is stored and 0 is
   returned. Otherwise out = (in.xyz * (max / len), in.w) and 1 is returned. */
s32 clamp_vector_length(void *out, void *in, f32 max) __asm__("FUN_001f9c90");
s32 clamp_vector_length(void *out, void *in, f32 max) {
    f32 v[4], len, rest;
    s32 len_bits, rest_bits;
    __builtin_memcpy(v, in, sizeof(v));
    len = __builtin_sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    rest = max - len;
    __builtin_memcpy(&len_bits, &len, 4);
    __builtin_memcpy(&rest_bits, &rest, 4);
    if (len_bits == 0 || rest_bits > 0) return 0;
    const f32 scale = max / len;
    for (s32 k = 0; k < 3; k++) v[k] *= scale;
    __builtin_memcpy(out, v, sizeof(v));
    return 1;
}

/* 0x125218 sceVu0UnitMatrix: vf4 = (0,0,0,1), each vmr32 rotates it one lane; stored as rows
   3..0, giving the 4x4 identity. */
void sce_vu0_unit_matrix(void *out) __asm__("FUN_00125218");
void sce_vu0_unit_matrix(void *out) { set_identity_matrix(out); }

/* libvu0 sceVu0RotMatrixX / Y / Z (0x125360 / 0x125408 / 0x1252b8) and their shared sine/cosine
   helper 0x125240. The caller makes t = pi/2 - a (a >= 0) or pi/2 + a (a < 0, c.lt.s); the helper
   takes the polynomial t + c3 t^3 + c2 t^5 + c1 t^7 + c0 t^9 (coefficients at 0x132e00, added in
   that order) as cos a, and sin a = +-sqrt(1 - cos^2) (vsqrt, |x|), negative when a < 0. Each
   routine builds columns m0..m3 from (c, s) and stores out[i] = m0 * in[i].x + m1 * in[i].y +
   m2 * in[i].z + m3 * in[i].w for the four rows (vmula / vmadda), so out may be in. */
static void vu0_cos_sin(f32 a, f32 *c, f32 *s) {
    static const u32 bits[4] = { 0x362e9c14, 0xb94fb21f, 0x3c08873e, 0xbe2aaaa4 };
    f32 k[4];
    __builtin_memcpy(k, bits, sizeof(k));
    const s32 negative = a < 0.0f;
    const f32 half_pi = 1.57079637f;
    const f32 t = negative ? half_pi + a : half_pi - a;
    const f32 t2 = t * t;
    const f32 t3 = t * t2, t5 = t3 * t2, t7 = t5 * t2, t9 = t7 * t2;
    f32 x = t + k[3] * t3;
    x = x + k[2] * t5;
    x = x + k[1] * t7;
    x = x + k[0] * t9;
    const f32 q = __builtin_sqrtf(__builtin_fabsf(1.0f - x * x));
    *c = x;
    *s = negative ? 0.0f - q : q;
}

static void vu0_apply_columns(f32 *out, const f32 *in, const f32 m[4][4]) {
    for (s32 row = 0; row < 4; row++) {
        f32 v[4], r[4];
        __builtin_memcpy(v, in + 4 * row, sizeof(v));
        for (s32 k = 0; k < 4; k++) r[k] = m[0][k] * v[0] + m[1][k] * v[1] + m[2][k] * v[2] + m[3][k] * v[3];
        __builtin_memcpy(out + 4 * row, r, sizeof(r));
    }
}

void sce_vu0_rot_matrix_x(f32 *out, f32 *in, f32 a) __asm__("FUN_00125360");
void sce_vu0_rot_matrix_x(f32 *out, f32 *in, f32 a) {
    f32 c, s;
    vu0_cos_sin(a, &c, &s);
    const f32 m[4][4] = { { 1, 0, 0, 0 }, { 0, c, s, 0 }, { 0, -s, c, 0 }, { 0, 0, 0, 1 } };
    vu0_apply_columns(out, in, m);
}
void sce_vu0_rot_matrix_y(f32 *out, f32 *in, f32 a) __asm__("FUN_00125408");
void sce_vu0_rot_matrix_y(f32 *out, f32 *in, f32 a) {
    f32 c, s;
    vu0_cos_sin(a, &c, &s);
    const f32 m[4][4] = { { c, 0, -s, 0 }, { 0, 1, 0, 0 }, { s, 0, c, 0 }, { 0, 0, 0, 1 } };
    vu0_apply_columns(out, in, m);
}
void sce_vu0_rot_matrix_z(f32 *out, f32 *in, f32 a) __asm__("FUN_001252b8");
void sce_vu0_rot_matrix_z(f32 *out, f32 *in, f32 a) {
    f32 c, s;
    vu0_cos_sin(a, &c, &s);
    const f32 m[4][4] = { { c, s, 0, 0 }, { -s, c, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
    vu0_apply_columns(out, in, m);
}

/* 0x22c4c8 sky_bsphere_check(tiles, count, visibility): A = D_0018CF80 (rows v0..v2, v3.w a
   scale, then f0 at +0x40 and f2 at +0x60), S = D_001D96E0 (rows s0..s2). r_k = v0 * s_k.x +
   v1 * s_k.y + v2 * s_k.z. For each 32-byte tile, p = tile.xyzw * v3.w, q = r0 * p.x + r1 * p.y +
   r2 * p.z, and for x and y: d = f0 * q.z - (|q| - f2 * p.w). visibility[i] = 0 when either d has
   its sign bit set (y tested first), else 1. count >= 1 (the caller returns on 0). */
extern f32 sky_view_block[8][4] __asm__("D_0018CF80");
extern f32 sky_shell_matrix[4][4] __asm__("D_001D96E0");
void sky_bsphere_check(const f32 *tiles, s32 count, u8 *visibility) __asm__("FUN_0022c4c8");
void sky_bsphere_check(const f32 *tiles, s32 count, u8 *visibility) {
    f32 r[3][3];
    for (s32 k = 0; k < 3; k++)
        for (s32 j = 0; j < 3; j++)
            r[k][j] = sky_view_block[0][j] * sky_shell_matrix[k][0] + sky_view_block[1][j] * sky_shell_matrix[k][1] +
                      sky_view_block[2][j] * sky_shell_matrix[k][2];
    const f32 scale = sky_view_block[3][3];
    do {
        f32 p[4];
        for (s32 k = 0; k < 4; k++) p[k] = tiles[k] * scale;
        f32 q[3];
        for (s32 j = 0; j < 3; j++) q[j] = r[0][j] * p[0] + r[1][j] * p[1] + r[2][j] * p[2];
        s32 visible = 1;
        for (s32 j = 1; j >= 0; j--) {
            const f32 d = sky_view_block[4][j] * q[2] - (__builtin_fabsf(q[j]) - sky_view_block[6][j] * p[3]);
            if (__builtin_signbit(d)) visible = 0;
        }
        *visibility++ = (u8)visible;
        tiles += 8;
    } while (--count != 0);
}

/* 0x22bf94 sky_transform_vertices(vertices, out, count, unused): C = M * S, M the view matrix at
   D_00186F40 + 0x100 (four columns), S the sky matrix D_001D96E0 (four rows: C's k-th
   column is M * S_k). Each 8-byte vertex is s16 x, y, z, a colour byte at +6. p = C * (x, y, z, 1)
   (vitof0); clip flags of p.xyz against |p.w| (vclipw: bit 0 +x, 1 -x, 2 +y, 3 -y, 4 +z, 5 -z;
   & 0x2f drops +z); p.xyz /= p.w; s = p * A + B, A = D_0018CD00 + 0x190 with x, y scaled by 0.25,
   B = D_0018CD00 + 0x1A0. out (16 bytes) = vftoi4 s.xyz, w 0, then byte +0xC = the colour and +0xF = the
   flags. Returns the AND of 0x3f and every vertex's flags (nonzero: all outside one plane). The
   assembly overlaps each vertex's load with the previous store; that order changes nothing. */
/* Named from their blocks' bases (D_00186F40 + 0x100, D_0018CD00 + 0x190 / + 0x1A0): those
   bases have level addresses (exe-data-l00.json); the interior addresses are not paired. */
extern u8 view_block[] __asm__("D_00186F40");
extern u8 sky_screen_block[] __asm__("D_0018CD00");
#define sky_view_columns (*(f32 (*)[4][4])(view_block + 0x100))
#define sky_screen_scale ((f32 *)(sky_screen_block + 0x190))
#define sky_screen_offset ((f32 *)(sky_screen_block + 0x1A0))
s32 sky_transform_vertices(const s16 *vertices, s32 *out, s32 count, s32 unused) __asm__("FUN_0022bf94");
s32 sky_transform_vertices(const s16 *vertices, s32 *out, s32 count, s32 unused) {
    (void)unused;
    f32 c[4][4];
    for (s32 k = 0; k < 4; k++)
        for (s32 j = 0; j < 4; j++)
            c[k][j] = sky_view_columns[0][j] * sky_shell_matrix[k][0] + sky_view_columns[1][j] * sky_shell_matrix[k][1] +
                      sky_view_columns[2][j] * sky_shell_matrix[k][2] + sky_view_columns[3][j] * sky_shell_matrix[k][3];
    f32 scale[4] = { sky_screen_scale[0] * 0.25f, sky_screen_scale[1] * 0.25f, sky_screen_scale[2], sky_screen_scale[3] };
    s32 all = 0x3f;
    do {
        const f32 v[3] = { (f32)vertices[0], (f32)vertices[1], (f32)vertices[2] };
        const u8 colour = ((const u8 *)vertices)[6];
        f32 p[4];
        for (s32 j = 0; j < 4; j++) p[j] = c[0][j] * v[0] + c[1][j] * v[1] + c[2][j] * v[2] + c[3][j];
        const f32 w = __builtin_fabsf(p[3]);
        u32 flags = 0;
        for (s32 j = 0; j < 3; j++) {
            if (p[j] > w) flags |= 1u << (2 * j);
            if (p[j] < -w) flags |= 2u << (2 * j);
        }
        flags &= 0x2f;
        const f32 q = 1.0f / p[3];
        for (s32 j = 0; j < 3; j++) out[j] = ee_ftoi((p[j] * q * scale[j] + sky_screen_offset[j]) * 16.0f);
        out[3] = 0;
        ((u8 *)out)[0xC] = colour;
        ((u8 *)out)[0xF] = (u8)flags;
        all &= (s32)flags;
        vertices += 4;
        out += 4;
    } while (--count != 0);
    return all;
}

/* 0x22c0e0 sky_build_gif_gouraud(count, indices, colours, vertices): GS packets at the render
   cursor (D_00160F00, gp - 0x5d00). Each triangle is 4 index bytes (3 used); it is left out when
   the AND of its three vertices' clip-flag bytes (+0xF of sky_transform_vertices' 16-byte output)
   is nonzero. Otherwise, per vertex: RGBAQ (A+D register 0x01) = the colour word sign-extended
   (lw, sd), then XYZ2 (0x05) = x & 0xffff | (y & 0xffff) << 16, z 0. With n quadwords written
   after the two header quadwords (none: the cursor is left as it was): DMA cnt tag of n + 1
   quadwords, VIF NOP, VIF DIRECT n + 1, GIF tag NLOOP n, EOP, packed, one register, A+D. */
extern u8 *render_cursor_bytes __asm__("D_00160F00");
void sky_build_gif_gouraud(s32 count, const u8 *indices, const s32 *colours, const s32 *vertices) __asm__("FUN_0022c0e0");
void sky_build_gif_gouraud(s32 count, const u8 *indices, const s32 *colours, const s32 *vertices) {
    u8 *const start = render_cursor_bytes;
    u8 *at = start + 0x20;
    do {
        const u8 *tri = indices;
        indices += 4;
        const u8 culled = ((const u8 *)(vertices + 4 * tri[0]))[0xF] & ((const u8 *)(vertices + 4 * tri[1]))[0xF] &
                          ((const u8 *)(vertices + 4 * tri[2]))[0xF];
        if (culled) continue;
        for (s32 k = 0; k < 3; k++) {
            const s32 *v = vertices + 4 * tri[k];
            *(s64 *)(at + 0x00) = (s64)colours[tri[k]];
            *(u64 *)(at + 0x08) = 1;
            *(u32 *)(at + 0x10) = ((u32)v[0] & 0xffff) | ((u32)v[1] & 0xffff) << 16;
            *(u32 *)(at + 0x14) = 0;
            *(u64 *)(at + 0x18) = 5;
            at += 0x20;
        }
    } while (--count != 0);
    const u32 n = (u32)((at - start) >> 4) - 2;
    if (n == 0) return;
    *(u32 *)(start + 0x00) = 0x10000000u | (n + 1);
    *(u32 *)(start + 0x04) = 0;
    *(u32 *)(start + 0x08) = 0;
    *(u32 *)(start + 0x0C) = 0x50000000u | (n + 1);
    *(u32 *)(start + 0x10) = n | 0x8000u;
    *(u32 *)(start + 0x14) = 0x10000000u;
    *(u64 *)(start + 0x18) = 0xe;
    render_cursor_bytes = at;
}

/* 0x22c208 sky_build_gif_textured(count, indices, uvs, vertices): as sky_build_gif_gouraud, with
   the triangle's fourth index byte a texture of the sky (D_0016045C, gp - 0x67a4: +0x10 a table
   of 16-byte entries { TEX0, +8 image, +0xA palette, +0xC log2 width, +0xE log2 height }, +0x14 the
   texture data base, quadword offsets). A texture other than the current one (initially 0xffff)
   gets TEX0_1 (A+D 0x06). A zero TEX0 is built on first use: GS memory from the allocator
   D_0015EE74 (palette 0x400 bytes, then 1 << (w + h) bytes), TBP0 = image >> 8, TBW = 1 << max(w
   - 6, 0), PSMT8, TW w, TH h, TCC 1, CBP = palette >> 8, CLD 4; and an upload (D_0018D040[count++],
   count D_0015F458) { palette source, 0, CBP, image source, w, h, TBP0 }. Per vertex: ST (0x02) =
   (u / 4096, v / 4096) from the u16 pair in uvs (pextlh, vitof12); RGBAQ (0x01) = 0x808080 with
   alpha the vertex byte +0xC, Q 1.0; XYZ2 (0x05) as in the gouraud builder. */
struct SkyTextureHeader { u8 pad[0x10]; u8 *textures; u8 *data; };
extern struct SkyTextureHeader *sky_texture_header __asm__("D_0016045C");
extern u32 gs_texture_cursor __asm__("D_0015EE74");
extern s32 texture_upload_count __asm__("D_0015F458");
extern u8 texture_uploads[0x40][16] __asm__("D_0018D040");
void sky_build_gif_textured(s32 count, const u8 *indices, const u32 *uvs, const s32 *vertices) __asm__("FUN_0022c208");
void sky_build_gif_textured(s32 count, const u8 *indices, const u32 *uvs, const s32 *vertices) {
    u8 *const start = render_cursor_bytes;
    u8 *at = start + 0x20;
    u32 current = 0xffff;
    do {
        const u8 *tri = indices;
        indices += 4;
        const u8 culled = ((const u8 *)(vertices + 4 * tri[0]))[0xF] & ((const u8 *)(vertices + 4 * tri[1]))[0xF] &
                          ((const u8 *)(vertices + 4 * tri[2]))[0xF];
        if (culled) continue;
        const u32 texture = tri[3];
        if (texture != current) {
            u8 *entry = sky_texture_header->textures + 16 * texture;
            u64 tex0 = *(u64 *)entry;
            if (tex0 == 0) {
                const s32 w = *(s16 *)(entry + 0xC), h = *(s16 *)(entry + 0xE);
                u32 cursor = gs_texture_cursor;
                const u32 palette = cursor >> 8;
                cursor += 0x400;
                const u32 image = cursor >> 8;
                cursor += 1u << (w + h);
                gs_texture_cursor = cursor;
                const s32 shift = w - 6 < 0 ? 0 : w - 6;
                tex0 = ((u64)(u32)(1 << shift) << 14) + image + ((u64)0x13 << 20) + ((u64)(s64)w << 26) +
                       ((u64)(s64)h << 30) + ((u64)1 << 34) + ((u64)palette << 37) + ((u64)4 << 61);
                *(u64 *)entry = tex0;
                u8 *upload = texture_uploads[texture_upload_count++];
                *(u32 *)(upload + 0) = (u32)(sky_texture_header->data + 16 * *(s16 *)(entry + 0xA));
                *(u16 *)(upload + 4) = 0;
                *(u16 *)(upload + 6) = (u16)palette;
                *(u32 *)(upload + 8) = (u32)(sky_texture_header->data + 16 * *(s16 *)(entry + 0x8));
                upload[0xC] = (u8)w;
                upload[0xD] = (u8)h;
                *(u16 *)(upload + 0xE) = (u16)image;
            }
            *(u64 *)(at + 0) = tex0;
            *(u64 *)(at + 8) = 6;
            at += 0x10;
            current = texture;
        }
        for (s32 k = 0; k < 3; k++) {
            const u32 uv = uvs[tri[k]];
            const s32 *v = vertices + 4 * tri[k];
            const f32 st[2] = { (f32)(s32)(uv & 0xffff) / 4096.0f, (f32)(s32)(uv >> 16) / 4096.0f };
            __builtin_memcpy(at + 0x00, st, 8);
            *(u64 *)(at + 0x08) = 2;
            *(u32 *)(at + 0x10) = (u32)((const u8 *)v)[0xC] << 24 | 0x808080u;
            *(u32 *)(at + 0x14) = 0x3f800000u;
            *(u64 *)(at + 0x18) = 1;
            *(u32 *)(at + 0x20) = ((u32)v[0] & 0xffff) | ((u32)v[1] & 0xffff) << 16;
            *(u32 *)(at + 0x24) = 0;
            *(u64 *)(at + 0x28) = 5;
            at += 0x30;
        }
    } while (--count != 0);
    const u32 n = (u32)((at - start) >> 4) - 2;
    if (n == 0) return;
    *(u32 *)(start + 0x00) = 0x10000000u | (n + 1);
    *(u32 *)(start + 0x04) = 0;
    *(u32 *)(start + 0x08) = 0;
    *(u32 *)(start + 0x0C) = 0x50000000u | (n + 1);
    *(u32 *)(start + 0x10) = n | 0x8000u;
    *(u32 *)(start + 0x14) = 0x10000000u;
    *(u64 *)(start + 0x18) = 0xe;
    render_cursor_bytes = at;
}

/* 0x22bba0 sky_sprite_proc: the sky's sprites (stars, sun) as VU1 sprite batches. Constants are
   read from the level copy (0x28bd58, gp = 0x166c00), whose addresses are used here directly:
   header D_0016045C (level 0x16051C): +0x08 u16 sprite count, +0x10 texture table, +0x14 texture
   data, +0x1C sprites (32 bytes: +1 visible flag, +2 s8 texture, +3 byte, +4 RGBA, +8 angle,
   +0x10 position xyz, +0x1C size). A = D_0018CD00 (level 0x16CA40), V = view block D_00186F40
   (level 0x166C80, rows 0-2 rotation, +0x40..+0x70 four quadwords), batch header / trailer / end
   quadwords at gp - 0x6460 / - 0x6450 / - 0x6440, the frame's two setup quadwords at gp - 0x6480 /
   - 0x6470, the TEX0 template at gp - 0x6720 (level addresses 0x1607A0, 0x1607B0, 0x1607C0,
   0x160780, 0x160790, 0x1604E0).
   Packet: DMA ref tag (0x30000000, qwc the half-word at 0x101070, address 0x101080), then 8
   quadwords: setup0, V+0x40..+0x60 times 1024 (all lanes), V+0x70, A+0x210, A+0x1A0, setup1.
   Then batches of up to 30 sprites (t4 counts 4 per sprite to 0x78): a header quadword (the batch
   header with qwc in its low half-word and qwc - 1 in byte 0xE; word +0x3C = 4 * sprites), 0x40
   bytes per sprite, the trailer quadword. A sprite with texture < 0, or outside the screen test
   (v = R * pos; for x and y: A+0xB0 * v.z - (|v| - A+0x1E0 * size) has its sign bit set) clears
   its visible flag; otherwise it sets it and writes { pos.xyz, 0 }, RGBA widened to four words
   (pextlb, pextlh), { Q cos(angle), Q sin(angle), byte +3 as a dword }, TEX0, 0, with Q = size *
   A+0x200.w / v.z. A texture whose TEX0 is not negative (bgezal: 0, not yet built) is built
   first by the inline routine at 0x22bec4: GS memory from the allocator (palette 0x400 bytes,
   then 1 << (w + h)), an upload record, TEX0 = template | TBP0 | 1 << max(w - 6, 0) << 14 |
   w << 26 | h << 30 | CBP << 37 (w, h bytes +0xC / +0xE; image +8, palette +0xA as u16). The end
   quadword closes the list. Q and the screen test use the assembly's operation order. */
extern u8 exe_00160760[] __asm__("D_00160760");
extern u8 exe_00101070[] __asm__("D_00101070");
extern u8 exe_0013E050[] __asm__("D_0013E050");
extern u8 exe_0015EE74[] __asm__("D_0015EE74");
extern u8 exe_0015F478[] __asm__("D_0015F478");
extern u8 exe_0015F47C[] __asm__("D_0015F47C");
extern u8 exe_0015F480[] __asm__("D_0015F480");
extern u8 exe_0015F604[] __asm__("D_0015F604");
extern u8 exe_0015FF00[] __asm__("D_0015FF00");
extern u8 exe_0015FF08[] __asm__("D_0015FF08");
extern u8 exe_0015FF30[] __asm__("D_0015FF30");
extern u8 exe_001603CC[] __asm__("D_001603CC");
extern u8 exe_001603EC[] __asm__("D_001603EC");
extern u8 exe_00160420[] __asm__("D_00160420");
extern u8 exe_0016045C[] __asm__("D_0016045C");
extern u8 exe_001604A4[] __asm__("D_001604A4");
extern u8 exe_001604A8[] __asm__("D_001604A8");
extern u8 exe_001604AC[] __asm__("D_001604AC");
extern u8 exe_00160520[] __asm__("D_00160520");
extern u8 exe_00160530[] __asm__("D_00160530");
extern u8 exe_00160540[] __asm__("D_00160540");
extern u8 exe_00160550[] __asm__("D_00160550");
extern u8 exe_00160560[] __asm__("D_00160560");
extern u8 exe_00160570[] __asm__("D_00160570");
extern u8 exe_001606C0[] __asm__("D_001606C0");
extern u8 exe_001606D0[] __asm__("D_001606D0");
extern u8 exe_001606E0[] __asm__("D_001606E0");
extern u8 exe_001606F0[] __asm__("D_001606F0");
extern u8 exe_00160700[] __asm__("D_00160700");
extern u8 exe_001607A0[] __asm__("D_001607A0");
extern u8 exe_00160E94[] __asm__("D_00160E94");
extern u8 exe_00160F4C[] __asm__("D_00160F4C");
extern u8 exe_00160F64[] __asm__("D_00160F64");
extern u8 exe_00186F40[] __asm__("D_00186F40");
extern u8 exe_00187080[] __asm__("D_00187080");
extern u8 exe_0018CD00[] __asm__("D_0018CD00");
extern u8 exe_00193FC0[] __asm__("D_00193FC0");
extern u8 exe_001B5980[] __asm__("D_001B5980");
extern u8 exe_001B5D80[] __asm__("D_001B5D80");
extern u8 exe_001B6180[] __asm__("D_001B6180");
extern u8 exe_001B6500[] __asm__("D_001B6500");
extern u8 exe_001B6880[] __asm__("D_001B6880");
extern u8 exe_001D84B0[] __asm__("D_001D84B0");
extern u8 exe_001D88B0[] __asm__("D_001D88B0");
extern u8 exe_001D8CB0[] __asm__("D_001D8CB0");
extern u8 exe_001D8DB0[] __asm__("D_001D8DB0");
extern u8 exe_001D92B0[] __asm__("D_001D92B0");
extern u8 exe_001DC4E0[] __asm__("D_001DC4E0");
extern u8 exe_001DCB40[] __asm__("D_001DCB40");
extern u8 exe_001DCE70[] __asm__("D_001DCE70");
extern u8 exe_001E0900[] __asm__("D_001E0900");
extern u8 exe_001E0C00[] __asm__("D_001E0C00");
extern u8 exe_001E2600[] __asm__("D_001E2600");
extern u8 exe_001E2A00[] __asm__("D_001E2A00");
extern u8 exe_001E2E00[] __asm__("D_001E2E00");
extern u8 exe_001E3000[] __asm__("D_001E3000");
extern u8 exe_001E3600[] __asm__("D_001E3600");
/* Fixed-address tables the ports read are named by the executable's address (exe_XXXXXXXX ->
   D_00XXXXXX): ram-image.mjs pins each at that address in the boot program and at the level
   program's copy in a level program (exe-data-l00.json), as it does for the recovered sources. */
void sky_sprite_proc(void) __asm__("FUN_0022bba0");
void sky_sprite_proc(void) {
    u8 *header = *(u8 **)((u8 *)exe_0016045C);
    const u8 *a = ((u8 *)exe_0018CD00);
    const u8 *view = ((u8 *)exe_00186F40);
    u8 *sprite = *(u8 **)(header + 0x1C);
    s32 left = *(u16 *)(header + 0x08);
    u8 *data = *(u8 **)(header + 0x14);
    u8 *textures = *(u8 **)(header + 0x10);
    if (left == 0) return;
    u8 *at = render_cursor_bytes;
    const f32 *a200 = (const f32 *)(a + 0x200), *ab0 = (const f32 *)(a + 0xB0), *a1e0 = (const f32 *)(a + 0x1E0);
    const f32 *r = (const f32 *)view;
    __builtin_memset(at, 0, 16);
    *(u32 *)(at + 4) = 0x101080;
    *(u32 *)(at + 0) = 0x30000000;
    *(u16 *)(at + 0) = (u16)*(u32 *)((u8 *)exe_00101070);
    at += 0x10;
    __builtin_memcpy(at + 0x00, ((u8 *)exe_001606C0), 16);
    __builtin_memcpy(at + 0x70, ((u8 *)exe_001606D0), 16);
    __builtin_memcpy(at + 0x50, a + 0x210, 16);
    __builtin_memcpy(at + 0x60, a + 0x1A0, 16);
    for (s32 row = 0; row < 3; row++) {
        f32 q[4];
        __builtin_memcpy(q, view + 0x40 + 16 * row, 16);
        for (s32 k = 0; k < 4; k++) q[k] *= 1024.0f;
        __builtin_memcpy(at + 0x10 + 16 * row, q, 16);
    }
    __builtin_memcpy(at + 0x40, view + 0x70, 16);
    at += 0x80;
    s32 batch = 0;
    u8 *batch_header = at;
    at += 0x10;
    for (; left > 0; left--, sprite += 0x20) {
        f32 pos[4], size;
        __builtin_memcpy(pos, sprite + 0x10, 16);
        __builtin_memcpy(&size, sprite + 0x1C, 4);
        const u8 shown = sprite[1];
        const s8 texture = (s8)sprite[2];
        const u32 rgba = *(u32 *)(sprite + 4);
        const u8 extra = sprite[3];
        f32 angle;
        __builtin_memcpy(&angle, sprite + 8, 4);
        f32 v[3];
        for (s32 j = 0; j < 3; j++) v[j] = r[0 + j] * pos[0] + r[4 + j] * pos[1] + r[8 + j] * pos[2];
        s32 inside = texture >= 0;
        if (inside) {
            for (s32 j = 1; j >= 0; j--) {
                const f32 d = ab0[j] * v[2] - (__builtin_fabsf(v[j]) - a1e0[j] * size);
                if (__builtin_signbit(d)) inside = 0;
            }
        }
        if (!inside) {
            if (shown) sprite[1] = 0;
            continue;
        }
        const f32 q = size * a200[3] / v[2];
        u8 *entry = textures + 16 * texture;
        s64 tex0 = *(s64 *)entry;
        __builtin_memcpy(at, pos, 16);
        if (!shown) sprite[1] = 1;
        const f32 st[2] = { q * fast_cos(angle), q * fast_sin(angle) };
        *(u32 *)(at + 0x0C) = 0;
        for (s32 k = 0; k < 4; k++) *(u32 *)(at + 0x10 + 4 * k) = (rgba >> (8 * k)) & 0xff;
        __builtin_memcpy(at + 0x20, st, 8);
        *(u64 *)(at + 0x28) = extra;
        if (tex0 >= 0) {
            u8 *upload = texture_uploads[texture_upload_count++];
            u32 cursor = gs_texture_cursor;
            const u32 palette = cursor >> 8;
            cursor += 0x400;
            const u32 image = cursor >> 8;
            const u32 w = entry[0xC], h = entry[0xE];
            *(u16 *)(upload + 6) = (u16)palette;
            *(u16 *)(upload + 0xE) = (u16)image;
            upload[0xC] = (u8)w;
            upload[0xD] = (u8)h;
            cursor += (1u << w) << h;
            *(u16 *)(upload + 4) = 0;
            gs_texture_cursor = cursor;
            *(u32 *)(upload + 0) = (u32)(data + 16 * *(u16 *)(entry + 0xA));
            *(u32 *)(upload + 8) = (u32)(data + 16 * *(u16 *)(entry + 0x8));
            const s32 shift = (s32)w - 6 < 0 ? 0 : (s32)w - 6;
            tex0 = (s64)(*(u64 *)((u8 *)exe_00160420) | image | (u64)(1u << shift) << 14 | (u64)w << 26 |
                         (u64)h << 30 | (u64)palette << 37);
            *(s64 *)entry = tex0;
        }
        *(s64 *)(at + 0x30) = tex0;
        *(u64 *)(at + 0x38) = 0;
        batch += 4;
        at += 0x40;
        if (batch - 0x78 < 0) continue;
        __builtin_memcpy(at, ((u8 *)exe_001606F0), 16);
        const u32 qwc = (u32)(at - batch_header) >> 4;
        at += 0x10;
        __builtin_memcpy(batch_header, ((u8 *)exe_001606E0), 16);
        *(u16 *)batch_header = (u16)qwc;
        batch_header[0xE] = (u8)(qwc - 1);
        *(s32 *)(batch_header + 0x3C) = batch;
        batch = 0;
        batch_header = at;
        at += 0x10;
    }
    at -= 0x10;
    if (batch != 0) {
        at += 0x10;
        __builtin_memcpy(at, ((u8 *)exe_001606F0), 16);
        const u32 qwc = (u32)(at - batch_header) >> 4;
        at += 0x10;
        __builtin_memcpy(batch_header, ((u8 *)exe_001606E0), 16);
        *(u16 *)batch_header = (u16)qwc;
        batch_header[0xE] = (u8)(qwc - 1);
        *(s32 *)(batch_header + 0x3C) = batch;
    }
    __builtin_memcpy(at, ((u8 *)exe_00160700), 16);
    at += 0x10;
    render_cursor_bytes = at;
}

/* 0x20b4a8 flush_texture_uploads (level copy 0x24d760, whose addresses are used: templates at
   gp - 0x63e0 .. - 0x63a0 = 0x160820 .. 0x160860): each queued upload (D_0018D040, count
   D_0015F458; the count is not reset here) becomes one 0x70-byte GS transfer for its palette,
   when it has one, then one for its image. A transfer is the five template quadwords at +0x00 ..
   +0x30 and +0x50 with +0x40 = 0 and word +0x48 = 0x53 (TRXDIR, host to local), BITBLTBUF's DBP
   (+0x24), DBW (+0x26) and DPSM (+0x27) patched, TRXREG RRW / RRH at +0x30 / +0x34, the image GIF
   tag's NLOOP | EOP at +0x50, then a DMA ref tag (+0x60: 0x30000000 + qwc, +0x64 the source)
   with VIF DIRECT qwc at +0x6C. Palette: DPSM = record u16 +4, 16 x 16, qwc 0x40 >> (DPSM >> 1),
   DBP = CBP. Image (8-bit): DBP = TBP0, DBW = 1 << (w - 6) only when w > 6 (else the template's),
   RRW 1 << w, RRH 1 << h, qwc ((1 << h) << w) >> 4. */
static u8 *texture_transfer(u8 *at, u32 source, u32 qwc) {
    for (s32 k = 0; k < 4; k++) __builtin_memcpy(at + 16 * k, (u8 *)exe_00160760 + 16 * k /* level copy 0x160820 */, 16);
    __builtin_memset(at + 0x40, 0, 16);
    __builtin_memcpy(at + 0x50, ((u8 *)exe_001607A0), 16);
    *(u32 *)(at + 0x48) = 0x53;
    *(u32 *)(at + 0x50) = qwc | 0x8000;
    __builtin_memset(at + 0x60, 0, 16);
    *(u32 *)(at + 0x64) = source;
    *(u32 *)(at + 0x60) = 0x30000000u + qwc;
    *(u32 *)(at + 0x6C) = 0x50000000u + qwc;
    return at;
}
void flush_texture_uploads(void) __asm__("FUN_0020b4a8");
void flush_texture_uploads(void) {
    u8 *at = render_cursor_bytes;
    s32 left = texture_upload_count;
    for (u8 *record = texture_uploads[0]; left != 0; left--, record += 16) {
        const u32 palette = *(u32 *)(record + 0);
        if (palette != 0) {
            const u32 psm = *(u16 *)(record + 4);
            const u32 qwc = 0x40u >> (psm >> 1);
            texture_transfer(at, palette, qwc);
            at[0x27] = (u8)psm;
            *(u16 *)(at + 0x24) = *(u16 *)(record + 6);
            *(u32 *)(at + 0x30) = 16;
            *(u32 *)(at + 0x34) = 16;
            at += 0x70;
        }
        const u32 w = record[0xC], h = record[0xD];
        const u32 qwc = ((1u << h) << w) >> 4;
        texture_transfer(at, *(u32 *)(record + 8), qwc);
        *(u16 *)(at + 0x24) = *(u16 *)(record + 0xE);
        if ((s32)w - 6 > 0) at[0x26] = (u8)(1u << (w - 6));
        *(u32 *)(at + 0x34) = 1u << h;
        *(u32 *)(at + 0x30) = 1u << w;
        at += 0x70;
    }
    render_cursor_bytes = at;
}

/* 0x234d48 build_tfrag_texture_dma(gs_address) (level copy 0x295100, whose addresses are used:
   texture count 0x160F54, texture words 0x1C4A00, table copy 0x1C4D00). The scratchpad table at
   0x2000 (0x300 bytes, two u16 GS block addresses per texture) is cleared. For each texture whose
   usage byte (scratchpad 0x3C00, set by the tfrag pass) is nonzero: word = log2 size << 28 |
   address. Bit 1 of the usage: its full-size 8-bit image (1 << n square) goes out first as a GS
   transfer (texture_transfer's layout, DBW 1 << (n - 6) when n > 6); then always the next mip
   level (n - 1), read from just past the full-size image (also when that was not sent). Each records its block address (the running GS block
   cursor, gs_address >> 8) in the table and advances the cursor by its size in 256-byte blocks;
   a transfer that takes the cursor past 0x3ffb is dropped and the walk ends. The table is copied
   to main memory by DMA channel 8 (scratchpad to memory, 0x30 quadwords) and the final cursor
   << 8 is returned. */
s32 build_tfrag_texture_dma(s32 gs_address) __asm__("FUN_00234d48");
s32 build_tfrag_texture_dma(s32 gs_address) {
    u8 *spr = (u8 *)0xF0000;
    u32 block = (u32)gs_address >> 8;
    u8 *at = render_cursor_bytes;
    const u8 *usage = spr + 0x3C00;
    const u8 *usage_end = usage + *(s32 *)((u8 *)exe_00160E94);
    const u32 *words = (const u32 *)((u8 *)exe_001E0900);
    u8 *table = spr + 0x2000;
    __builtin_memset(table, 0, 0x300);
    for (;; usage++, words++) {
        table += 4;
        if (usage == usage_end) break;
        const s8 use = (s8)*usage;
        if (use == 0) continue;
        u32 n = *words >> 28;
        u32 source = *words & 0x0FFFFFFF;
        s32 overflow = 0;
        for (s32 level = (use & 2) ? 0 : 1; level < 2; level++) {
            if (level == 1) {
                n -= 1;
                source += ((1u << n) << n) << 2; /* add a0, a0, at runs on both paths */
            }
            const u32 qwc = ((1u << n) << n) >> 4;
            texture_transfer(at, source, qwc);
            *(u16 *)(at + 0x24) = (u16)block;
            if ((s32)n - 6 > 0) at[0x26] = (u8)(1u << (n - 6));
            *(u32 *)(at + 0x30) = 1u << n;
            *(u32 *)(at + 0x34) = 1u << n;
            *(u16 *)(table - 4 + 2 * level) = (u16)block;
            block += qwc >> 4;
            if ((s32)block - 0x3ffb > 0) { overflow = 1; break; }
            at += 0x70;
        }
        if (overflow) break;
    }
    render_cursor_bytes = at;
    __builtin_memcpy(((u8 *)exe_001E0C00), spr + 0x2000, 0x30 * 16);
    return (s32)(block << 8);
}

/* Shared by the tie and tfrag texture builders: one GS image transfer of an 8-bit square texture
   (1 << n per side) into GS block `block` (DBW 1 << (n - 6) when n > 6). */
static void square_texture_transfer(u8 *at, u32 source, u32 n, u32 block) {
    const u32 qwc = ((1u << n) << n) >> 4;
    texture_transfer(at, source, qwc);
    *(u16 *)(at + 0x24) = (u16)block;
    if ((s32)n - 6 > 0) at[0x26] = (u8)(1u << (n - 6));
    *(u32 *)(at + 0x30) = 1u << n;
    *(u32 *)(at + 0x34) = 1u << n;
}

/* build_tie_texture_dma (0x2370c0, level copy 0x297478), build_shrub_texture_dma (0x22a330,
   level copy 0x288658) and build_moby_texture_dma (below) are the same code on different tables (below, from the level copies).
   Described with the tie tables:
   Per tie class i (count 0x160FBC): d = scratchpad 0x3200[i] (the nearest visible distance from
   tie_proc, 0x7fffffff: none), lod = 0x1C6F00[i]. Skipped when d is 0x7fffffff or 2 lod - d <= 0;
   otherwise i is appended to the visible-class list at 0x1C7100 and each texture byte of the
   class's 16-byte record at 0x1C7700 (0xFF ends it) gets usage flags 3 (d < lod: both mip levels)
   or 1 (the smaller one only) ORed into scratchpad 0x3E00 (cleared, 0x100 bytes). The list ends
   with -1. Then per texture (count 0x160FD4; words 0x1C6700 = log2 size << 28 | address) its two
   u16 GS block addresses at 0x1C6B00 are cleared and, for a used texture, transfers are built as
   in build_tfrag_texture_dma (full size when usage bit 1, then always the next mip level, read
   from past the full-size image); a transfer past block 0x3ffb is dropped and ends the walk.
   Returns the final block << 8. */
struct ClassTextureTables {
    u32 hidden;                                                    /* "not visible" distance */
    u32 distances, usage;                                          /* scratchpad offsets */
    u8 *lods, *visible, *records, *class_count, *texture_count, *words, *table; /* executable labels */
};
static s32 build_class_texture_dma(s32 gs_address, const struct ClassTextureTables *k) {
    u8 *spr = (u8 *)0xF0000;
    const u32 *distances = (const u32 *)(spr + k->distances);
    u8 *usage = spr + k->usage;
    const s32 *lods = (const s32 *)(k->lods);
    s32 *visible = (s32 *)(k->visible);
    const u8 *records = (k->records);
    const s32 classes = *(const s32 *)(k->class_count);
    u32 block = (u32)gs_address >> 8;
    u8 *at = render_cursor_bytes;
    __builtin_memset(usage, 0, 0x100);
    for (s32 i = 0; i != classes; i++) {
        const s32 d = (s32)distances[i], lod = lods[i];
        if ((u32)d == k->hidden) continue;
        *visible = i;
        if (2 * lod - d <= 0) continue;
        const u8 flags = lod - d <= 0 ? 1 : 3;
        visible++;
        for (const u8 *texture = records + 16 * i; *texture != 0xff; texture++) usage[*texture] |= flags;
    }
    *visible = -1;
    const s32 textures = *(const s32 *)(k->texture_count);
    const u32 *words = (const u32 *)(k->words);
    u16 *table = (u16 *)(k->table);
    for (s32 t = 0; t != textures; t++, table += 2) {
        const u8 use = usage[t];
        *(u32 *)table = 0;
        if (use == 0) continue;
        u32 n = words[t] >> 28;
        u32 source = words[t] & 0x0FFFFFFF;
        s32 overflow = 0;
        if (use & 2) {
            square_texture_transfer(at, source, n, block);
            table[0] = (u16)block;
            block += (((1u << n) << n) >> 4) >> 4;
            if ((s32)block - 0x3ffb > 0) overflow = 1;
            else at += 0x70;
        }
        if (!overflow) {
            n -= 1;
            source += ((1u << n) << n) << 2;
            square_texture_transfer(at, source, n, block);
            table[1] = (u16)block;
            block += (((1u << n) << n) >> 4) >> 4;
            if ((s32)block - 0x3ffb > 0) overflow = 1;
            else at += 0x70;
        }
        if (overflow) break;
    }
    render_cursor_bytes = at;
    return (s32)(block << 8);
}
static const struct ClassTextureTables tie_texture_tables = {
    0x7fffffff, 0x3200, 0x3E00, exe_001E2E00, exe_001E3000, exe_001E3600, exe_00160F4C, exe_00160F64, exe_001E2600, exe_001E2A00 };
static const struct ClassTextureTables shrub_texture_tables = {
    0x7fffffff, 0x3100, 0x3600, exe_001D8CB0, exe_001D8DB0, exe_001D92B0, exe_001603CC, exe_001603EC, exe_001D84B0, exe_001D88B0 };
s32 build_tie_texture_dma(s32 gs_address) __asm__("FUN_002370c0");
s32 build_tie_texture_dma(s32 gs_address) { return build_class_texture_dma(gs_address, &tie_texture_tables); }
s32 build_shrub_texture_dma(s32 gs_address) __asm__("FUN_0022a330");
s32 build_shrub_texture_dma(s32 gs_address) { return build_class_texture_dma(gs_address, &shrub_texture_tables); }
/* build_moby_texture_dma (0x211408, level copy 0x2555c0): the same code with 0x40000000 as the
   "not visible" distance (the moby class distances init_moby_class_dists sets). */
static const struct ClassTextureTables moby_texture_tables = {
    0x40000000, 0x3A00, 0x3E00, exe_001B6180, exe_001B6500, exe_001B6880, exe_0015FF00, exe_0015FF08, exe_001B5980, exe_001B5D80 };
s32 build_moby_texture_dma(s32 gs_address) __asm__("FUN_00211408");
s32 build_moby_texture_dma(s32 gs_address) { return build_class_texture_dma(gs_address, &moby_texture_tables); }

/* 0x1f8ff0 prepare_glow_sprites (level copy 0x1fe6d8, whose addresses are used; called by
   draw_debug_font when sprites are queued). DMA channel 9 copies 0x10 quadwords from 0x1802C0
   to scratchpad 0x2400. GS memory for one shared palette (0x400 bytes, CBP) is taken from the
   allocator 0x15EE74. Then for each of [0x15F438] 32-byte items ([0x15F43C]; item +0x0 position,
   +0xC radius, +0x10 near distance) with texture [[0x15F440][i]] + 4 (header { log2 w, log2 h },
   pixels at +0x10): v = fast_bsphere_check(item, near + 16 - radius); +0x14 = 0, and when v >= 0:
   z = view-space depth of the position (view rows at 0x166C80, camera at +0x140; pexew takes z),
   alpha = 0xff when z <= near, else (int)(255 - (z - near) * (255 / 16)); bit 31 set when v == 0;
   stored at +0x14. ASSUMED: the near case is `ori a1, s6, 0xff`, which keeps the caller's s6
   above the low byte; 0 is taken for s6. A visible item (v >= 0) then gets GS memory
   (1 << (w + h) bytes), TEX0 at +0x18 (PSMT8, TBW 1: the delay slot of the max(w - 6, 0) branch
   zeroes it, TCC 1, CBP, CLD 4) and an upload record (palette 0x16E480, CBP, pixels, w, h, TBP0);
   the guard on the queue (`count + 0x40 < 0`) never skips. The VU0
   register the assembly leaves set (vf30.xyz = 1) has no reader in the ported code. */
void WriteDmaChannelRegisters(s32 address, s32 qwc, s32 destination);
s32 fast_bsphere_check(void *sphere, f32 range) __asm__("FUN_001fa728");
void prepare_glow_sprites(void) __asm__("FUN_001f8ff0");
void prepare_glow_sprites(void) {
    WriteDmaChannelRegisters(0x1802C0, 0x10, 0x2400);
    u32 *cursor = (u32 *)((u8 *)exe_0015EE74);
    const u32 palette = *cursor >> 8;
    *cursor += 0x400;
    s32 count = *(s32 *)((u8 *)exe_0015F478);
    u8 *item = *(u8 **)((u8 *)exe_0015F47C);
    u8 **list = *(u8 ***)((u8 *)exe_0015F480);
    const f32 *view = (const f32 *)((u8 *)exe_00186F40);
    const f32 per_unit = 255.0f / 16.0f;
    do {
        u8 *texture_entry = *list;
        f32 near, radius;
        __builtin_memcpy(&near, item + 0x10, 4);
        __builtin_memcpy(&radius, item + 0x0C, 4);
        const s32 v = fast_bsphere_check(item, near + 16.0f - radius);
        *(u32 *)(item + 0x14) = 0;
        if (v >= 0) {
            f32 p[3];
            __builtin_memcpy(p, item, 12);
            for (s32 k = 0; k < 3; k++) p[k] -= view[0x50 + k];
            const f32 z = view[2] * p[0] + view[6] * p[1] + view[10] * p[2];
            u32 alpha = z <= near ? 0xff : (u32)ee_ftoi(255.0f - (z - near) * per_unit);
            if (v == 0) alpha |= 0x80000000u;
            *(u32 *)(item + 0x14) = alpha;
            const u8 *header = *(u8 **)(texture_entry + 4);
            const u32 w = *(const u32 *)header, h = *(const u32 *)(header + 4);
            const u32 image = *cursor >> 8;
            *cursor += 1u << (w + h);
            const u64 tex0 = image + ((u64)1 << 14) + ((u64)0x13 << 20) + ((u64)w << 26) + ((u64)h << 30) +
                             ((u64)1 << 34) + ((u64)palette << 37) + ((u64)0x1000 << 51);
            *(u64 *)(item + 0x18) = tex0;
            u8 *upload = texture_uploads[texture_upload_count];
            *(u32 *)(upload + 0) = 0x16E480;
            *(u16 *)(upload + 4) = 0;
            *(u16 *)(upload + 6) = (u16)palette;
            *(u32 *)(upload + 8) = (u32)(header + 0x10);
            upload[0xC] = (u8)w;
            upload[0xD] = (u8)h;
            *(u16 *)(upload + 0xE) = (u16)image;
            texture_upload_count += 1;
        }
        item += 0x20;
        list += 1;
    } while (--count != 0);
}

/* 0x1f9ff8 set_scale_matrix(out, s): rows (s,0,0,0), (0,s,0,0), (0,0,s,0), (0,0,0,1). */
void set_scale_matrix(f32 *out, f32 s) __asm__("FUN_001f9ff8");
void set_scale_matrix(f32 *out, f32 s) {
    const f32 m[16] = { s, 0, 0, 0, 0, s, 0, 0, 0, 0, s, 0, 0, 0, 0, 1 };
    __builtin_memcpy(out, m, sizeof(m));
}

/* moby_proc (0x211808, assembly; level copy 0x2559c0, whose addresses are used), split in two.
   Its per-moby culling writes moby +0x31 ("visible": drawn last frame), which 194 gameplay files
   read, so that half is ported here. The rest builds the VU1 moby packets, the class distance
   table and the animation queue moby_anim_proc skins from; that half is handed to the browser's
   moby renderer, like the other VU1 passes: no packets, and the queue head comes back unchanged
   (gameplay bone lookups compute poses directly, build_moby_bone_transform); a zero word at the
   head ends it, as moby_anim_proc expects. ASSUMED, not
   verified: nothing outside rendering reads the queue or the skinning matrices it produces.
   Mobys are 0x100 bytes from `mobys` until `mobys + count * 0x100` (count -1 in draw_mobys: the
   list's end, state -1, stops it). A negative state skips the moby (+0x31 untouched). Otherwise,
   with sphere s = +0x00 (xyz, w radius r), v = R * (s.xyz - 1024 * camera) (view block 0x166C80:
   rows 0-2, camera at +0x140), z = v.z, D = (float)(min(+0x32 draw distance, cap) << 10) (cap at
   0x15FFF0) and A = 0x16CA40, the moby is visible when all hold, in the assembly's order:
   (+0x36 & regions[+0x37]) != 0 (region masks at 0x173D00, the scratchpad copy's source);
   (+0x34 flags & 0x81) == 0; (D - r) - (z - r) has its sign bit clear (in draw distance);
   A[0xA0].x - (r + z) has its sign bit set (past the near limit); and for x and y,
   A[0xB0] * z - (|v| - A[0x1E0] * r) has its sign bit clear (on screen). Mobys with flags &
   0xC00 (when `special`) take another path for packets, with the same rule for +0x31. Visible:
   +0x31 = 1 when it was 0; not visible: +0x31 = 0 when it was not. */
s32 moby_proc(u8 *mobys, s32 head, s32 count, s32 special) __asm__("FUN_00211808");
s32 moby_proc(u8 *mobys, s32 head, s32 count, s32 special) {
    (void)special;
    const f32 *view = (const f32 *)((u8 *)exe_00186F40);
    const f32 *a = (const f32 *)((u8 *)exe_0018CD00);
    const u8 *regions = ((u8 *)exe_00193FC0);
    const s32 cap = *(const s32 *)((u8 *)exe_0015FF30);
    f32 cam[3];
    for (s32 k = 0; k < 3; k++) cam[k] = view[0x50 + k] * 1024.0f;
    u8 *const end = mobys + count * 0x100;
    for (u8 *m = mobys; m != end; m += 0x100) {
        const s8 state = (s8)m[0x20];
        if (state < 0) {
            if (state == -1) break;
            continue;
        }
        const s16 draw = *(s16 *)(m + 0x32);
        const s32 limited = draw < cap ? draw : cap;
        f32 s[4];
        __builtin_memcpy(s, m, 16);
        const f32 r = s[3];
        f32 d[3], v[3];
        for (s32 k = 0; k < 3; k++) d[k] = s[k] - cam[k];
        for (s32 k = 0; k < 3; k++) v[k] = view[k] * d[0] + view[4 + k] * d[1] + view[8 + k] * d[2];
        const f32 z = v[2];
        const f32 far = (f32)(s32)((u32)limited << 10) - r;
        s32 visible = 1;
        if ((m[0x36] & regions[m[0x37]]) == 0) visible = 0;
        else if (*(u16 *)(m + 0x34) & 0x81) visible = 0;
        else {
            const f32 near_test = a[0xA0 / 4] - (r + z);
            const f32 far_test = far - (z + -r);
            if (__builtin_signbit(far_test) || !__builtin_signbit(near_test)) visible = 0;
            else
                for (s32 k = 1; k >= 0; k--) {
                    const f32 edge = a[0xB0 / 4 + k] * z - (__builtin_fabsf(v[k]) - a[0x1E0 / 4 + k] * r);
                    if (__builtin_signbit(edge)) visible = 0;
                }
        }
        if (visible) { if (m[0x31] == 0) m[0x31] = 1; }
        else if (m[0x31] != 0) m[0x31] = 0;
    }
    *(u32 *)head = 0; /* the animation queue, empty: moby_anim_proc stops at a zero word */
    return head;
}

/* 0x211728 moby_anim_proc(queue, arg): while the queue word is nonzero, 0x20e0e0(entry, arg)
   skins it and 0x1ee650(entry, arg) returns the next entry; then it waits for DMA channel 8
   (done at once here). Registers are saved at 0x1B7680; nothing else is written. */
void moby_skin_entry(u32 *entry, s32 arg) __asm__("FUN_0020e0e0");
u32 *moby_next_entry(u32 *entry, s32 arg) __asm__("FUN_001ee650");
void moby_anim_proc(u32 *queue, s32 arg) __asm__("FUN_00211728");
void moby_anim_proc(u32 *queue, s32 arg) {
    while (*queue != 0) {
        moby_skin_entry(queue, arg);
        queue = moby_next_entry(queue, arg);
    }
}

/* Level 0 0x2935b8, the draw callback clank_cutscene_trigger registers for class 0x212 mobys on
   Veldin (compiled code with no C or INCLUDE_ASM in the recovered tree; decompiled from the
   level program). An environment-mapped mesh: per class (index oclass - 0x212) the level keeps
   tables at gp - 0x65d0 .. - 0x6620 (0x160630 quads, 0x160620 vertices, 0x160610 normals,
   0x160600 quad count, 0x1605F0 vertex count, 0x1605E0 colour). Vertices go through the moby's
   bone 0 matrix (build_moby_bone_transform) into 0x1C05E0. The texture coordinates (0x1C0C40)
   are recomputed, from the reflection of the camera direction about each normal (normal scaled
   to 0.1, sphere map: m = 2 sqrt(2 (r.z + 1)), uv = r.xy / m + 0.5), while the mesh is near the
   camera (|dx| and |dy| < 16 with level mode 0), always in any other mode except mode 6 with
   [0x13E050] == 4, or while state 0x160564 is 1; while the timer 0x16056C runs (and the state is
   not 1) they ease from the previous coordinates (0x1C0F70) by timer / scale_game_frames(60).
   Otherwise (state 0x160568 == 1 the first frame) the previous coordinates are saved and the
   timer restarts. Every quad (four s16 vertex indices, 16 bytes) is drawn by fast_draw_quad_real
   from a stack record { 4 positions, 4 colours, 4 uv pairs, 0, TEX0 (get_effect_texture 1 in
   mode 6 with [0x13E050] == 4, else 0x15), 0xFF90_00000260, 0x80_00000044 }. */
u64 get_effect_texture(s32 index) __asm__("FUN_001f44b8");
f32 absolute_float(f32 x) __asm__("FUN_001f99c0");
f32 convert_integer_to_float(s32 i) __asm__("FUN_001fa6c0");
void build_moby_bone_transform(void *moby, s32 bone, void *out) __asm__("FUN_0020cca8");
void fast_draw_quad_real(void *quad, s32 a, s32 b) __asm__("FUN_001f7d30");
struct EnvQuad {
    f32 pos[4][4];
    u32 colour[4];
    f32 uv[4][2];
    u64 zero, tex0, word80, word88;
};
void draw_class_0x212_env_mesh(u8 *moby) __asm__("FUN_L00_002935b8");
void draw_class_0x212_env_mesh(u8 *moby) {
    const s32 idx = *(s16 *)(moby + 0xA6) - 0x212;
    const s16 *quads = ((s16 **)((u8 *)exe_00160570))[idx];
    u8 *verts = ((u8 **)((u8 *)exe_00160560))[idx];
    u8 *normals = ((u8 **)((u8 *)exe_00160550))[idx];
    const s32 vcount = ((s32 *)((u8 *)exe_00160530))[idx];
    const s32 qcount = ((s32 *)((u8 *)exe_00160540))[idx];
    const s32 mode = *(s32 *)((u8 *)exe_0015F604);
    const s32 special = *(s32 *)((u8 *)exe_0013E050);
    s32 *state = (s32 *)((u8 *)exe_001604A4), *first = (s32 *)((u8 *)exe_001604A8);
    s32 *timer = (s32 *)((u8 *)exe_001604AC);
    u8 *world = ((u8 *)exe_001DC4E0);
    f32 *uv = (f32 *)((u8 *)exe_001DCB40), *previous = (f32 *)((u8 *)exe_001DCE70);
    const f32 *cam = (const f32 *)((u8 *)exe_00187080);
    struct EnvQuad q;
    f32 matrix[16];
    q.tex0 = get_effect_texture(mode == 6 && special == 4 ? 1 : 0x15);
    q.word88 = 0x0000008000000044ull;
    q.word80 = 0x0000FF9000000260ull;
    for (s32 k = 0; k < 4; k++) q.colour[k] = ((u32 *)((u8 *)exe_00160520))[idx];
    q.zero = 0;
    build_moby_bone_transform(moby, 0, matrix);
    s32 near = 0;
    if (mode != 0) near = 1;
    else {
        const f32 *pos = (const f32 *)(moby + 0x10);
        if (absolute_float(cam[0] - pos[0]) < 16.0f && absolute_float(cam[1] - pos[1]) < 16.0f) near = 1;
    }
    if (mode == 6 && special == 4) near = 0;
    if (near || *state == 1) {
        *first = 1;
        fast_dec_timer(timer);
        const f32 blend = convert_integer_to_float(*timer) / convert_integer_to_float(scale_game_frames(0x3c));
        for (s32 i = 0; i < vcount; i++) {
            f32 dir[4], n[4], r[4];
            u8 *out = world + 16 * i;
            transform_vector(out, verts + 16 * i, matrix);
            fast_vec_sub(dir, out, (void *)cam);
            fast_vec_normalize(dir, dir, 1.0f);
            transform_vector(n, normals + 16 * i, matrix);
            fast_vec_normalize(n, n, 0.1f);
            const f32 d = fast_vec_dot(n, dir);
            fast_vec_scale(r, n, d + d);
            fast_vec_sub(r, dir, r);
            fast_vec_normalize(r, r, 1.0f);
            r[2] = r[2] + 1.0f;
            const f32 m = square_root(r[2] + r[2]);
            const f32 m2 = m + m;
            const f32 u = r[0] / m2 + 0.5f, v = r[1] / m2 + 0.5f;
            if (*state == 1 || *timer == 0) {
                uv[2 * i] = u;
                uv[2 * i + 1] = v;
            } else {
                uv[2 * i] = u + (previous[2 * i] - u) * blend;
                uv[2 * i + 1] = v + (previous[2 * i + 1] - v) * blend;
            }
        }
        if (*state == 1) *state = 2;
    } else {
        if (*first == 1) {
            *first = 0;
            for (s32 i = 0; i < vcount; i++) {
                previous[2 * i] = uv[2 * i];
                previous[2 * i + 1] = uv[2 * i + 1];
                transform_vector(world + 16 * i, verts + 16 * i, matrix);
            }
        } else {
            for (s32 i = 0; i < vcount; i++) transform_vector(world + 16 * i, verts + 16 * i, matrix);
        }
        *timer = scale_game_frames(0x3c);
    }
    for (s32 i = 0; i < qcount; i++) {
        const s16 *quad = (const s16 *)((const u8 *)quads + 16 * i);
        for (s32 k = 0; k < 4; k++) {
            const s32 v = quad[2 * k];
            __builtin_memcpy(q.pos[k], world + 16 * v, 16);
            q.uv[k][0] = uv[2 * v];
            q.uv[k][1] = uv[2 * v + 1];
        }
        fast_draw_quad_real(&q, 0, 0);
    }
}

/* The boot ELF's libc, named by address in the recovered sources (config/us/symbol_addrs.txt:
   strncpy 0x116808, memcpy 0x115248, memset 0x1153fc). The browser's libc (linked) has the same
   C semantics. */
char *elf_strncpy(char *to, const char *from, u32 n) __asm__("FUN_00116808");
char *elf_strncpy(char *to, const char *from, u32 n) { return __builtin_strncpy(to, from, n); }
void *elf_memcpy(void *to, const void *from, u32 n) __asm__("FUN_00115248");
void *elf_memcpy(void *to, const void *from, u32 n) { return __builtin_memcpy(to, from, n); }
void *elf_memset(void *to, s32 value, u32 n) __asm__("FUN_001153fc");
void *elf_memset(void *to, s32 value, u32 n) { return __builtin_memset(to, value, n); }

/* Boot sources call the VU0 library by its Sony names. */
ALIAS(sce_vu0_unit_matrix, sceVu0UnitMatrix, FUN_00125218)
ALIAS(sce_vu0_rot_matrix_x, sceVu0RotMatrixX, FUN_00125360)
ALIAS(sce_vu0_rot_matrix_y, sceVu0RotMatrixY, FUN_00125408)
ALIAS(sce_vu0_rot_matrix_z, sceVu0RotMatrixZ, FUN_001252b8)
ALIAS(sce_vu0_rot_matrix_x, SceVu0RotMatrixX, FUN_00125360)
ALIAS(sce_vu0_rot_matrix_y, SceVu0RotMatrixY, FUN_00125408)
ALIAS(sce_vu0_rot_matrix_z, SceVu0RotMatrixZ, FUN_001252b8)
ALIAS(sce_vu0_normalize, sceVu0Normalize, FUN_00125180)

/* Moby collision grid (assembly: 0x20db40, 0x20dba0, update_moby_grids 0x20dc20). 64 x 64 cells
   of 4 bytes at D_001B76E0 { u16 first block, u8 count, u8 capacity in blocks }; 32-byte blocks
   of u16 moby ids at cells + 0x4000; a bitmap of free blocks at D_001C76E0. A moby's rectangle of
   cells is the word at +0xA0 { min x, min y, max x, max y } (negative: in no cell), its id the
   half-word at +0xAC. */
extern u8 moby_grid_cells[0x4000] __asm__("D_001B76E0");
extern u8 moby_grid_bitmap[] __asm__("D_001C76E0");
#define MOBY_GRID_BLOCKS (moby_grid_cells + 0x4000)

/* 0x20db40: clears `count` bits from `index`; a bit already clear stops the console (teq). */
static void grid_free_blocks(u32 index, s32 count) {
    do {
        const u8 bit = (u8)(1u << (index & 7));
        u8 *byte = &moby_grid_bitmap[index >> 3];
        if ((*byte & (u8)~bit) == *byte) __builtin_trap();
        *byte &= (u8)~bit;
        index += 1;
    } while (--count > 0);
}

/* 0x20dba0: the first run of `count` clear bits that starts at a multiple of `count` within one
   32-bit word of the bitmap (words that are all set are skipped), set; returns its index. */
static u32 grid_alloc_blocks(u32 count) {
    const u64 mask = (1ull << count) - 1;
    u32 base = 0;
    for (u32 *word = (u32 *)moby_grid_bitmap;; word++, base += 32) {
        const u32 bits = *word;
        if (bits == 0xFFFFFFFFu) continue;
        u32 at = 0;
        u64 run = mask;
        while ((bits & run) != 0) { run <<= count; at += count; }
        if (at == 32) continue;
        *word = bits | (u32)run;
        return base + at;
    }
}

static s32 grid_inside(u32 x, u32 y, u32 key) {
    /* psubsb of (x, y, x, y) and the rectangle, as update_moby_grids tests it */
    const s32 dx0 = (s8)(u8)x - (s8)(u8)key, dy0 = (s8)(u8)y - (s8)(u8)(key >> 8);
    const s32 dx1 = (s8)(u8)x - (s8)(u8)(key >> 16), dy1 = (s8)(u8)y - (s8)(u8)(key >> 24);
    return !(dx0 < 0 || dy0 < 0 || dx1 > 0 || dy1 > 0);
}

void update_moby_grids(u8 *moby, u32 key) __asm__("FUN_0020dc20");
void update_moby_grids(u8 *moby, u32 key) {
    const u16 id = (u16)*(u32 *)(moby + 0xAC);
    const u32 old = *(u32 *)(moby + 0xA0);
    if (!(old & 0x80000000u)) {
        for (u32 y = (old >> 8) & 0xFF;; y++) {
            for (u32 x = old & 0xFF;; x++) {
                if (!grid_inside(x, y, key)) {
                    u8 *cell = moby_grid_cells + x * 4 + y * 256;
                    u16 *ids = (u16 *)(MOBY_GRID_BLOCKS + *(u16 *)cell * 32);
                    u16 *end = ids + cell[2];
                    u16 *at = ids;
                    for (;; at++) {
                        if (at == end) __builtin_trap(); /* not in its cell (teq) */
                        if (*at == id) break;
                    }
                    *at = end[-1];
                    end[-1] = 0;
                    const u8 count = (u8)(cell[2] - 1), capacity = cell[3];
                    cell[2] = count;
                    if (count == 0 || (capacity != 1 && (s32)(capacity * 8 - 4) - count > 0)) {
                        const s16 from = *(s16 *)cell;
                        grid_free_blocks(*(u16 *)cell, capacity);
                        *(u16 *)cell = 0;
                        cell[3] = (u8)(capacity >> 1);
                        if (cell[3] != 0) {
                            const u32 to = grid_alloc_blocks(cell[3]);
                            *(u16 *)cell = (u16)to;
                            __builtin_memmove(MOBY_GRID_BLOCKS + to * 32, MOBY_GRID_BLOCKS + from * 32, 32u * cell[3]);
                        }
                    }
                }
                if (x == ((old >> 16) & 0xFF)) break;
            }
            if (y == (old >> 24)) break;
        }
    }
    *(u32 *)(moby + 0xA0) = key;
    if (key & 0x80000000u) return;
    for (u32 y = (key >> 8) & 0xFF;; y++) {
        for (u32 x = key & 0xFF;; x++) {
            if (!grid_inside(x, y, old)) {
                u8 *cell = moby_grid_cells + x * 4 + y * 256;
                if (cell[3] == 0 || (s32)(cell[3] * 16) - cell[2] <= 0) {
                    const u8 capacity = cell[3];
                    const u16 from = *(u16 *)cell;
                    cell[3] = (u8)(capacity * 2 > 1 ? capacity * 2 : 1);
                    *(u16 *)cell = (u16)grid_alloc_blocks(cell[3]);
                    if (capacity != 0) {
                        grid_free_blocks(from, capacity);
                        __builtin_memmove(MOBY_GRID_BLOCKS + *(u16 *)cell * 32, MOBY_GRID_BLOCKS + from * 32, 32u * (cell[3] >> 1));
                    }
                }
                const u8 count = cell[2];
                cell[2] = (u8)(count + 1);
                *(u16 *)(MOBY_GRID_BLOCKS + *(u16 *)cell * 32 + count * 2) = id;
            }
            if (x == ((key >> 16) & 0xFF)) break;
        }
        if (y == (key >> 24)) break;
    }
}

/* update_entity / refresh_moby_spatial_bounds (0x20def8; the level copy at 0x250df8 is the same
   code on the level's tables). For a moby with state >= 0: the rotation rows at +0xC0..+0xE0 are
   kept when flags & 0x100, else built from the euler angles at +0x40 (VU0 microprogram 0x1a3,
   euler_rows above; ASSUMED equivalent, as there). The class bounding sphere comes from the
   current sequence (+0x52; pclass + 0x48 lists sequence pointers, a sequence's first quadword is
   its sphere): cached at +0xF0 when +0x52 equals +0x71 (+0x71 then follows +0x52), or, while
   blending from +0x53, A + (B - A) * t with t = +0x54 (sequence 0xFF takes A from D_001B2C00
   indexed by +0x50). flags & 0x8000 negates the middle row. The world sphere at +0x00 is
   rows * (sphere * scale +0x2C) + 1024 * position (+0x10), w = radius * scale; +0xA8 counts
   updates. When the moby is in the grid (+0x94 != 0), its cell rectangle is (sphere.xy -/+
   radius, as integers) >> 14 packed as bytes; a change, with min x and min y below 0x40, goes to
   update_moby_grids. */
extern u8 moby_sequence_spheres[] __asm__("D_001B2C00");
static void euler_rows(f32 rows[3][4], const void *angles);
static void update_entity_rows(u8 *moby, s32 keep_basis);
void update_entity(u8 *moby) __asm__("FUN_0020def8");
void update_entity(u8 *moby) { update_entity_rows(moby, 0); }
/* 0x20e098 refresh_moby_spatial_bounds_from_basis: the same with the rows always kept from
   +0xC0 (it loads them and joins update_entity at 0x20df30, as flags & 0x100 does). */
void refresh_moby_spatial_bounds_from_basis(u8 *moby) __asm__("FUN_0020e098");
void refresh_moby_spatial_bounds_from_basis(u8 *moby) { update_entity_rows(moby, 1); }
static void update_entity_rows(u8 *moby, s32 keep_basis) {
    if ((s8)moby[0x20] < 0) return;
    const u16 flags = *(u16 *)(moby + 0x34);
    u8 *pclass = *(u8 **)(moby + 0x24);
    f32 rows[3][4];
    if (keep_basis || (flags & 0x100)) __builtin_memcpy(rows, moby + 0xC0, sizeof(rows));
    else euler_rows(rows, moby + 0x40);
    const u8 seq = moby[0x52], prev = moby[0x53];
    f32 sphere[4];
    if (seq != prev) {
        f32 a[4], b[4], t;
        if (seq == 0xFF) __builtin_memcpy(a, moby_sequence_spheres + 16 * moby[0x50], 16);
        else __builtin_memcpy(a, *(u8 **)(pclass + 0x48 + 4 * seq), 16);
        __builtin_memcpy(b, *(u8 **)(pclass + 0x48 + 4 * prev), 16);
        __builtin_memcpy(&t, moby + 0x54, 4);
        for (s32 k = 0; k < 4; k++) sphere[k] = (b[k] * t + a[k]) - a[k] * t;
    } else if (seq != moby[0x71]) {
        moby[0x71] = seq;
        __builtin_memcpy(sphere, *(u8 **)(pclass + 0x48 + 4 * seq), 16);
        __builtin_memcpy(moby + 0xF0, sphere, 16);
    } else {
        __builtin_memcpy(sphere, moby + 0xF0, 16);
    }
    if (flags & 0x8000) for (s32 k = 0; k < 3; k++) rows[1][k] = -rows[1][k];
    __builtin_memcpy(moby + 0xC0, rows, sizeof(rows));
    f32 scale, pos[3], s[4];
    __builtin_memcpy(&scale, moby + 0x2C, 4);
    __builtin_memcpy(pos, moby + 0x10, 12);
    for (s32 k = 0; k < 4; k++) s[k] = sphere[k] * scale;
    f32 world[4];
    for (s32 k = 0; k < 3; k++) world[k] = rows[0][k] * s[0] + rows[1][k] * s[1] + rows[2][k] * s[2] + pos[k] * 1024.0f;
    world[3] = s[3];
    __builtin_memcpy(moby, world, 16);
    *(u16 *)(moby + 0xA8) = (u16)(*(u16 *)(moby + 0xA8) + 1);
    if (*(u32 *)(moby + 0x94) == 0) return;
    const s32 cx = ee_ftoi(world[0]), cy = ee_ftoi(world[1]), r = ee_ftoi(world[3]);
    const u32 key = ((u32)((cx - r) >> 14) & 0xFF) | (((u32)((cy - r) >> 14) & 0xFF) << 8) |
                    (((u32)((cx + r) >> 14) & 0xFF) << 16) | (((u32)((cy + r) >> 14) & 0xFF) << 24);
    if (key == *(u32 *)(moby + 0xA0) || (key & 0xC0C0)) return;
    update_moby_grids(moby, key);
}
ALIAS(update_entity, FUN_L00_00250df8, FUN_0020def8)
/* The boot program's copy of the visibility list (the menu system's moby update, 0x20d868). */
extern u8 *build_resident_visibility_list(void) __asm__("FUN_L00_00250768");
u8 *boot_build_resident_visibility_list(void) __asm__("FUN_0020d868");
u8 *boot_build_resident_visibility_list(void) { return build_resident_visibility_list(); }

/* 0x2106f8: attachment point positions (browser/moby-joint-matrices.c). */
void get_moby_point_positions(void *moby, s32 count, s32 *points, void *out) __asm__("FUN_002106f8");
void get_moby_point_positions(void *moby, s32 count, s32 *points, void *out) {
    rnc_get_moby_point_positions(moby, count, points, out);
}

/* 0x1f99a8 "SubtractIntegerWithClamp" (symbol_addrs.txt's name): the integer absolute value
   (bgez; sub a0, zero, a0). The level copy is 0x1ff200 (level-copies.json). */
s32 integer_absolute(s32 x) __asm__("FUN_001f99a8");
s32 integer_absolute(s32 x) { return x >= 0 ? x : 0 - x; }
ALIAS(integer_absolute, SubtractIntegerWithClamp, FUN_001f99a8)

/* (max_float 0x1f99c8 is above.) 0x1f99d0 min_float (max.s / min.s; the EE has no NaN or infinity), and
   0x1f99d8 the smallest of three integers (two pminw on the low words). */
f32 min_float(f32 a, f32 b) __asm__("FUN_001f99d0");
f32 min_float(f32 a, f32 b) { return a < b ? a : b; }
s32 min_int3(s32 a, s32 b, s32 c) __asm__("FUN_001f99d8");
s32 min_int3(s32 a, s32 b, s32 c) { s32 m = a < b ? a : b; return m < c ? m : c; }

/* 0x219d80: a menu screen's update (no C in the recovered tree). menu_system is D_001D5BF0
   (+0x04 current page, page +0x40 the focused screen, +0x38 the page Triangle goes back to,
   +0x08 the next page, +0x124 a flag), the controller D_0013C940 (+0x1C4 buttons pressed this
   frame). For the focused screen only (else 0): START, SELECT or R3 (0xD00) return 1 unless the
   flag is set; then Triangle (0x10) requests the back page when there is one (returns 0), or
   returns -1 when the flag is clear; anything else returns 0. */
extern u8 menu_system_bytes[] __asm__("D_001D5BF0");
extern u8 controller_bytes[] __asm__("D_0013C940");
s32 menu_screen_confirm_or_back(void *screen) __asm__("FUN_00219d80");
s32 menu_screen_confirm_or_back(void *screen) {
    u8 *page = *(u8 **)(menu_system_bytes + 4);
    if (*(void **)(page + 0x40) != screen) return 0;
    const u32 pressed = *(u32 *)(controller_bytes + 0x1C4);
    const s32 flag = *(s32 *)(menu_system_bytes + 0x124);
    if ((pressed & 0xD00) && flag == 0) return 1;
    if (!(pressed & 0x10)) return 0;
    void *back = *(void **)(page + 0x38);
    if (back) { *(void **)(menu_system_bytes + 8) = back; return 0; }
    return flag == 0 ? -1 : 0;
}

/* 0x1f7d30 fast_draw_quad_real(quad, matrix, unused): one textured quad (VU0 macro code with a
   private register convention; the level program's copy is at 0x1fd228). Quad, 0x90 bytes:
   +0x00..+0x30 four points (vec4), +0x40 four RGBA words, +0x50 S0 T0 S1 T1, +0x60 S2 T2 S3 T3,
   +0x70 CLAMP_1, +0x78 TEX0_1, +0x80 TEX1_1, +0x88 ALPHA_1 (64-bit register values).
   Each point: p = matrix ? x*R0 + y*R1 + z*R2 + w*R3 (matrix rows R0..R3) : point; then
   xyz = (xyz - camera) * D_00160B20.z, camera = view block D_00186F40 + 0x140; then view space
   v = x*V0 + y*V1 + z*V2 + V3 (V at view block + 0x40, w of the input treated as 1).
   S = screen block D_0018CD00: +0x180 clip scale K, +0x190 screen scale, +0x1A0 screen offset.
   - Guard band: c = v * (K.x*G.y, K.y*G.y, K.z, K.w), G = D_00160B20; if every point is outside
     one plane (vclipw: +x x>|w|, -x x<-|w|, likewise y, z) nothing is drawn.
   - c = v * K. If no point is outside any plane: 12 quadwords at the render cursor
     (D_00160F00): DMA cnt + VIF DIRECT 12; GIF tag A+D x2 (TEX1_1, ALPHA_1); GIF tag REGLIST
     EOP, 15 registers CLAMP_1 TEX0_1 PRIM (ST RGBAQ XYZF2) x4, PRIM 0x7C (triangle strip,
     Gouraud, textured, fogged, blended). Per point Q = D_00160B30.x / v.w, ST = (S*Q, T*Q),
     RGBAQ = rgba | Q << 32, XYZF2 from s = v*Q + offset: x, y = ftoi4 & 0xffff, z = ftoi4 >> 4
     (word), fog byte = clamp(ftoi4(s.w) >> 4, 0, 255).
   - Otherwise: a state packet (5 quadwords: REGLIST CLAMP_1 TEX0_1; A+D TEX1_1, ALPHA_1), then
     the triangles (p0 p1 p2) and (p1 p2 p3), each clipped against the six planes of c in the
     order -z +z -y +y -x +x (Sutherland-Hodgman; 0x1f7a98 interpolates position, S, T as floats
     and RGBA per byte as (c*16 + 2) >> 4) and drawn as one triangle fan (PRIM 0x7D) with
     s = (c * screen scale) * Q + offset, Q = D_00160B30.x / (c * screen scale).w.
   ASSUMED: for clipped polygons of more than three points the assembly (0x1f8484-0x1f88bc,
   helpers 0x1f7cd0 / 0x1f7c10) picks the fan's first point by distance and may add one point on
   an edge; the fan here starts at the first clipped point. A convex fan covers the same pixels
   from any first point. The depth sum the routine computes (s3) is not used by its allocator. */
extern f32 quad_constants_a[4] __asm__("D_00160B20");
extern f32 quad_constants_b[4] __asm__("D_00160B30");
static s32 vu_ftoi4(f32 x) {
    const f32 v = x * 16.0f;
    if (!(v == v)) return 0;
    if (v >= 2147483520.0f) return 0x7fffffff;
    if (v <= -2147483648.0f) return (s32)0x80000000;
    return (s32)v;
}
static u32 vu_clip(const f32 *c) {
    const f32 w = __builtin_fabsf(c[3]);
    u32 flags = 0;
    for (s32 j = 0; j < 3; j++) {
        if (c[j] > w) flags |= 1u << (2 * j);
        if (c[j] < -w) flags |= 2u << (2 * j);
    }
    return flags;
}
static u8 *quad_alloc(s32 qwc) {
    u8 *at = render_cursor_bytes;
    __builtin_memset(at, 0, 16);
    *(u32 *)(at + 0) = 0x10000000u | (u32)qwc;          /* DMA cnt, qwc */
    *(u32 *)(at + 12) = 0x50000000u | (u32)qwc;         /* VIF DIRECT qwc (word 8: NOP) */
    render_cursor_bytes = at + 16 + qwc * 16;
    return at + 16;
}
static void quad_xyzf(u8 *out, const f32 *p, f32 q) {
    const f32 *offset = (const f32 *)(sky_screen_block + 0x1A0);
    const s32 x = vu_ftoi4(p[0] * q + offset[0]), y = vu_ftoi4(p[1] * q + offset[1]);
    const s32 z = vu_ftoi4(p[2] * q + offset[2]) >> 4, w = vu_ftoi4(p[3] * q + offset[3]) >> 4;
    *(u32 *)out = ((u32)x & 0xffff) | ((u32)y << 16);
    *(s32 *)(out + 4) = z;
    out[7] = (u8)(w > 255 ? 255 : w < 0 ? 0 : w);
}
typedef struct { f32 p[4]; f32 s, t; u32 rgba, pad; } QuadVertex;
static void quad_lerp(QuadVertex *out, const QuadVertex *a, const QuadVertex *b, s32 plane) {
    const s32 axis = plane >> 1;
    const f32 ca = (plane & 1) ? -a->p[axis] : a->p[axis], cb = (plane & 1) ? -b->p[axis] : b->p[axis];
    const f32 t = (ca - a->p[3]) / ((b->p[3] - a->p[3]) + (ca - cb));
    for (s32 j = 0; j < 4; j++) out->p[j] = a->p[j] * 1.0f + b->p[j] * t - a->p[j] * t;
    out->s = (b->s - a->s) * t + a->s;
    out->t = (b->t - a->t) * t + a->t;
    u32 rgba = 0;
    for (s32 k = 0; k < 4; k++) {
        const f32 ea = (f32)((a->rgba >> (8 * k)) & 0xff), eb = (f32)((b->rgba >> (8 * k)) & 0xff);
        const s32 v = (((vu_ftoi4(ea * 1.0f + eb * t - ea * t) & 0xffff) + 2) & 0xffff) >> 4;
        rgba |= ((u32)v & 0xff) << (8 * k);
    }
    out->rgba = rgba;
    out->pad = 0;
}
static void quad_state_packet(const u8 *quad) {
    u8 *at = quad_alloc(5);
    *(u64 *)(at + 0x00) = 0x2400000000000001ull; *(u64 *)(at + 0x08) = 0x68;
    *(u64 *)(at + 0x10) = *(const u64 *)(quad + 0x70); *(u64 *)(at + 0x18) = *(const u64 *)(quad + 0x78);
    *(u64 *)(at + 0x20) = 0x2000000000008001ull; *(u64 *)(at + 0x28) = 0xee;
    *(u64 *)(at + 0x30) = *(const u64 *)(quad + 0x80); *(u64 *)(at + 0x38) = 0x14;
    *(u64 *)(at + 0x40) = *(const u64 *)(quad + 0x88); *(u64 *)(at + 0x48) = 0x42;
}
static void quad_clip_triangle(QuadVertex *in) {
    QuadVertex a[12], b[12], *src = a, *dst = b;
    s32 n = 3;
    for (s32 k = 0; k < 3; k++) src[k] = in[k];
    for (s32 plane = 5; plane >= 0; plane--) {
        s32 m = 0;
        src[n] = src[0];
        for (s32 i = 0; i < n; i++) {
            const u32 fa = vu_clip(src[i].p), fb = vu_clip(src[i + 1].p);
            const s32 out_a = (fa >> plane) & 1, out_b = (fb >> plane) & 1;
            if (!out_a) dst[m++] = src[i];
            if (out_a != out_b) quad_lerp(&dst[m++], &src[i], &src[i + 1], plane);
        }
        n = m;
        if (n == 0) return;
        QuadVertex *swap = src; src = dst; dst = swap;
    }
    const f32 *scale = (const f32 *)(sky_screen_block + 0x190);
    u8 *at = quad_alloc((3 * n) / 2 + 3);
    *(u32 *)(at + 0) = 1; *(u32 *)(at + 4) = 0x44000000;      /* REGLIST, NREG 4, NLOOP 1 */
    *(u64 *)(at + 8) = 0x4120;                                  /* PRIM ST RGBAQ XYZF2 */
    *(u64 *)(at + 0x10) = 0x7d;                                 /* triangle fan */
    *(u32 *)(at + 0x30) = (u32)(n - 1) | 0x8000; *(u32 *)(at + 0x34) = 0x34000000;
    *(u64 *)(at + 0x38) = 0x412;                                /* ST RGBAQ XYZF2, NLOOP n-1, EOP */
    u8 *v = at + 0x18;
    for (s32 i = 0; i < n; i++) {
        f32 p[4];
        for (s32 j = 0; j < 4; j++) p[j] = src[i].p[j] * scale[j];
        const f32 q = quad_constants_b[0] / p[3];
        f32 st[2] = { src[i].s * q, src[i].t * q };
        __builtin_memcpy(v, st, 8);
        *(u32 *)(v + 8) = src[i].rgba;
        __builtin_memcpy(v + 12, &q, 4);
        quad_xyzf(v + 16, p, q);
        v += i == 0 ? 0x28 : 0x18;
    }
}
void fast_draw_quad_real(void *quad_record, s32 matrix_address, s32 unused) {
    const u8 *quad = (const u8 *)quad_record;
    const f32 *matrix = (const f32 *)matrix_address;
    (void)unused;
    const f32 *camera = (const f32 *)(view_block + 0x140), *V = (const f32 *)(view_block + 0x40);
    const f32 *K = (const f32 *)(sky_screen_block + 0x180);
    f32 v[4][4];
    for (s32 k = 0; k < 4; k++) {
        const f32 *in = (const f32 *)(quad + 16 * k);
        f32 p[4];
        for (s32 j = 0; j < 4; j++)
            p[j] = matrix ? matrix[j] * in[0] + matrix[4 + j] * in[1] + matrix[8 + j] * in[2] + matrix[12 + j] * in[3] : in[j];
        for (s32 j = 0; j < 3; j++) p[j] = (p[j] - camera[j]) * quad_constants_a[2];
        for (s32 j = 0; j < 4; j++) v[k][j] = V[j] * p[0] + V[4 + j] * p[1] + V[8 + j] * p[2] + V[12 + j];
    }
    const f32 guard[4] = { K[0] * quad_constants_a[1], K[1] * quad_constants_a[1], K[2], K[3] };
    u32 all = 0x3f, any = 0;
    f32 c[4][4];
    for (s32 k = 0; k < 4; k++) {
        f32 g[4];
        for (s32 j = 0; j < 4; j++) { g[j] = v[k][j] * guard[j]; c[k][j] = v[k][j] * K[j]; }
        all &= vu_clip(g);
        any |= vu_clip(c[k]);
    }
    if (all) return;
    const u32 *rgba = (const u32 *)(quad + 0x40);
    const f32 *st = (const f32 *)(quad + 0x50);
    if (any) {
        quad_state_packet(quad);
        QuadVertex tri[3];
        for (s32 first = 0; first < 2; first++) {
            for (s32 i = 0; i < 3; i++) {
                const s32 k = first + i;
                for (s32 j = 0; j < 4; j++) tri[i].p[j] = c[k][j];
                tri[i].s = st[2 * k]; tri[i].t = st[2 * k + 1]; tri[i].rgba = rgba[k]; tri[i].pad = 0;
            }
            quad_clip_triangle(tri);
        }
        return;
    }
    u8 *at = quad_alloc(12);
    *(u64 *)(at + 0x00) = 0x2000000000000001ull; *(u64 *)(at + 0x08) = 0xee;
    *(u64 *)(at + 0x10) = *(const u64 *)(quad + 0x80); *(u64 *)(at + 0x18) = 0x14;
    *(u64 *)(at + 0x20) = *(const u64 *)(quad + 0x88); *(u64 *)(at + 0x28) = 0x42;
    *(u64 *)(at + 0x30) = 0xF400000000008001ull; *(u64 *)(at + 0x38) = 0x0412412412412068ull;
    *(u64 *)(at + 0x40) = *(const u64 *)(quad + 0x70);
    *(u64 *)(at + 0x48) = *(const u64 *)(quad + 0x78);
    *(u64 *)(at + 0x50) = 0x7c;
    for (s32 k = 0; k < 4; k++) {
        u8 *out = at + 0x58 + 0x18 * k;
        const f32 q = quad_constants_b[0] / v[k][3];
        const f32 sq[2] = { st[2 * k] * q, st[2 * k + 1] * q };
        __builtin_memcpy(out, sq, 8);
        *(u32 *)(out + 8) = rgba[k];
        __builtin_memcpy(out + 12, &q, 4);
        quad_xyzf(out + 16, v[k], q);
    }
    *(u64 *)(at + 0xB8) = 0;
}

/* 0x1f96b0 scale_time(x): lwc1 f0, gp - 0x7e9c (D_0015ED64, resident data below the level
   program); return f0 * x. The game's time-scale factor. */
extern f32 time_scale_factor __asm__("D_0015ED64");
f32 scale_time(f32 x) __asm__("FUN_001f96b0");
f32 scale_time(f32 x) { return time_scale_factor * x; }

/* Small VU0 / MMI helpers (assembly-only), each read from its listing. */
/* 0x1f9a98: out = a * b, all four lanes. */
void multiply_vector_components(f32 *out, const f32 *a, const f32 *b) __asm__("FUN_001f9a98");
void multiply_vector_components(f32 *out, const f32 *a, const f32 *b) {
    f32 r[4];
    for (s32 k = 0; k < 4; k++) r[k] = a[k] * b[k];
    __builtin_memcpy(out, r, 16);
}
/* 0x1f9958: vclipw.xyz v, v.w; the six clip flags (+x -x +y -y +z -z against |w|). */
u32 is_vector_outside_clip(const f32 *v) __asm__("FUN_001f9958");
u32 is_vector_outside_clip(const f32 *v) { return vu_clip(v); }
/* 0x1f96e8: x * D_0015ED68 (gp - 0x7e98, resident), the companion of scale_time. */
extern f32 time_scale_factor_b __asm__("D_0015ED68");
f32 scale_time_b(f32 x) __asm__("FUN_001f96e8");
f32 scale_time_b(f32 x) { return time_scale_factor_b * x; }
/* 0x1f97c0: a busy-wait loop of a0 / 2 passes; nothing to wait for here. */
void spin_delay(s32 count) __asm__("FUN_001f97c0");
void spin_delay(s32 count) { (void)count; }
/* 0x1f9bb0: 1 when the spheres a and b (xyz centre, w radius) overlap: |a - b|^2 - (a.w + b.w)^2
   is negative (its sign bit: -0 counts), else 0. */
s32 spheres_overlap(const f32 *a, const f32 *b) __asm__("FUN_001f9bb0");
s32 spheres_overlap(const f32 *a, const f32 *b) {
    const f32 dx = a[0] - b[0], dy = a[1] - b[1], dz = a[2] - b[2], r = a[3] + b[3];
    const f32 d = ((dx * dx + dy * dy) + dz * dz) - r * r;
    return __builtin_signbit(d) ? 1 : 0;
}
/* 0x1f98f8: 16 bytes at a time, dst = a | b, while n > 0 (at least one quadword). */
void fast_mem_or16(u8 *dst, const u8 *a, const u8 *b, s32 n) __asm__("FUN_001f98f8");
void fast_mem_or16(u8 *dst, const u8 *a, const u8 *b, s32 n) {
    do {
        for (s32 k = 0; k < 16; k++) dst[k] = a[k] | b[k];
        dst += 16; a += 16; b += 16; n -= 16;
    } while (n > 0);
}
/* 0x1f9d68: vftoi0 v; the low byte of each lane, x in bits 0-7 ... w in bits 24-31. */
u32 fast_vector_to_packed_chars(const f32 *v) __asm__("FUN_001f9d68");
u32 fast_vector_to_packed_chars(const f32 *v) {
    u32 out = 0;
    for (s32 k = 0; k < 4; k++) out |= ((u32)ee_ftoi(v[k]) & 0xff) << (8 * k);
    return out;
}
/* 0x1f9d50: the four bytes of packed (unsigned) to floats. */
void fast_vector_from_packed_chars(f32 *out, u32 packed) __asm__("FUN_001f9d50");
void fast_vector_from_packed_chars(f32 *out, u32 packed) {
    f32 r[4];
    for (s32 k = 0; k < 4; k++) r[k] = (f32)((packed >> (8 * k)) & 0xff);
    __builtin_memcpy(out, r, 16);
}
/* 0x1fa480 (three rows) / 0x1fa4f8 (four, the last 0 0 0 1): the rotation of the quaternion
   q = (x, y, z, w), in the listing's order of operations:
   row0 = (1 - 2yy - 2zz, 2xy - 2zw, 2xz + 2yw, 0), row1 = (2zw + 2xy, 1 - 2xx - 2zz, 2yz - 2xw, 0),
   row2 = (2xz - 2yw, 2xw + 2yz, 1 - 2xx - 2yy, 0). */
static void quaternion_rows(f32 rows[3][4], const f32 *q) {
    const f32 x2 = q[0] + q[0], y2 = q[1] + q[1], z2 = q[2] + q[2];
    const f32 xw = x2 * q[3], yw = y2 * q[3], zw = z2 * q[3];
    const f32 xx = x2 * q[0], xy = y2 * q[0], xz = z2 * q[0];
    const f32 yy = y2 * q[1], yz = z2 * q[1], zz = z2 * q[2];
    rows[0][0] = (1.0f - yy) - zz; rows[0][1] = xy - zw; rows[0][2] = xz + yw; rows[0][3] = 0.0f;
    rows[1][0] = zw + xy; rows[1][1] = (1.0f - xx) - zz; rows[1][2] = yz - xw; rows[1][3] = 0.0f;
    rows[2][0] = (0.0f - yw) + xz; rows[2][1] = (0.0f + xw) + yz; rows[2][2] = (1.0f - xx) - yy; rows[2][3] = 0.0f;
}
void quaternion_to_rotation_basis(const f32 *q, f32 *out) __asm__("FUN_001fa480");
void quaternion_to_rotation_basis(const f32 *q, f32 *out) {
    f32 rows[3][4];
    quaternion_rows(rows, q);
    __builtin_memcpy(out, rows, sizeof(rows));
}
void quaternion_to_matrix(const f32 *q, f32 *out) __asm__("FUN_001fa4f8");
void quaternion_to_matrix(const f32 *q, f32 *out) {
    f32 rows[4][4];
    quaternion_rows(rows, q);
    rows[3][0] = rows[3][1] = rows[3][2] = 0.0f; rows[3][3] = 1.0f;
    __builtin_memcpy(out, rows, sizeof(rows));
}
/* 0x1fa400 quaternion_nlerp(out, a, b, t): p = a * (1 - t) + b * t, or a * (1 - t) - b * t when
   the products' sum (y + x + z + w, the listing's order) is negative; out = p / |p|. */
void quaternion_nlerp(f32 *out, const f32 *a, const f32 *b, f32 t) __asm__("FUN_001fa400");
void quaternion_nlerp(f32 *out, const f32 *a, const f32 *b, f32 t) {
    const f32 u = 1.0f - t;
    f32 p[4], q[4], s[4];
    for (s32 k = 0; k < 4; k++) { p[k] = a[k] * u; q[k] = b[k] * t; s[k] = p[k] + q[k]; }
    const f32 dot = ((p[1] * q[1] + p[0] * q[0]) + p[2] * q[2]) + p[3] * q[3];
    if (__builtin_signbit(dot)) for (s32 k = 0; k < 4; k++) s[k] = p[k] - q[k];
    const f32 n = ((s[0] * s[0] + s[1] * s[1]) + s[2] * s[2]) + s[3] * s[3];
    const f32 inv = 1.0f / __builtin_sqrtf(n);
    for (s32 k = 0; k < 4; k++) out[k] = s[k] * inv;
}

/* Menu screen callbacks with no recovered C (they were placeholders of the wrong type, so the
   menu system's call through the screen table trapped). Data addresses are the boot program's;
   0x1D62B8, 0x199712 and 0x1CFC40 have no pairing in exe-data-l00.json, so in the level program
   (pause menu) these read the boot addresses (UNVERIFIED there). */
extern u32 planet_order[] __asm__("D_0013D510");          /* planet ids, 0-terminated */
extern u8 planet_names[] __asm__("D_001DDD40");           /* 12-byte records: +0 text id, +4 info */
extern u8 planet_select_entries[] __asm__("D_001D62B8");  /* 12-byte menu text items */
extern u32 *planet_visit_list __asm__("D_001601E0");      /* 0-terminated */
extern u32 current_planet_key __asm__("D_001A0314");
/* 0x221800 (enter): up to 20 planets from planet_order into the item list (+0 name id, +2 = 1,
   +4 = 0, +8 info id), a 0 terminator; screen +0x40 (selection) = the index of current_planet_key
   in planet_visit_list (0 when absent) and screen +0x30 |= 0x8000. (v0 is 0; the menu system's
   enter slot is void, which is the type the screen table calls it with.) */
void enter_planet_select_list(u8 *screen, s32 arg) __asm__("FUN_00221800");
void enter_planet_select_list(u8 *screen, s32 arg) {
    (void)arg;
    s32 count = 0;
    while (count < 20 && planet_order[count] != 0) {
        u8 *entry = planet_select_entries + 12 * count;
        const u32 id = planet_order[count];
        *(u16 *)(entry + 2) = 1;
        *(u32 *)(entry + 4) = 0;
        *(u16 *)(entry + 8) = *(u16 *)(planet_names + id * 12 + 4);
        *(u16 *)(entry + 0) = *(u16 *)(planet_names + id * 12);
        count++;
    }
    *(u16 *)(planet_select_entries + 12 * count) = 0;
    *(s32 *)(screen + 0x40) = 0;
    *(u32 *)(screen + 0x30) |= 0x8000;
    for (s32 i = 0; planet_visit_list[i] != 0; i++) {
        if (planet_visit_list[i] == current_planet_key) { *(s32 *)(screen + 0x40) = i; break; }
    }
}
extern s32 reverse_list_count __asm__("D_0015EE30");
extern u8 reverse_list_ids[] __asm__("D_00141E08");
extern u8 reverse_list_text[] __asm__("D_00199712");      /* u16 text id every 4 bytes */
extern u8 reverse_list_entries[] __asm__("D_001CFC40");   /* 12-byte menu text items */
/* 0x21d168 (enter): for i < count, item i = { text of id byte [count - 1 - i], +2 = 1 }; then a
   0 terminator. (An enter slot: void, as above.) */
void enter_reverse_text_list(u8 *screen, s32 arg) __asm__("FUN_0021d168");
void enter_reverse_text_list(u8 *screen, s32 arg) {
    (void)screen; (void)arg;
    const s32 count = reverse_list_count;
    for (s32 i = 0; i < count; i++) {
        u8 *entry = reverse_list_entries + 12 * i;
        *(u16 *)(entry + 2) = 1;
        *(u16 *)(entry + 0) = *(u16 *)(reverse_list_text + 4 * reverse_list_ids[count - (i + 1)]);
    }
    *(u16 *)(reverse_list_entries + 12 * count) = 0;
}
extern s32 prompt_mode __asm__("D_0015EEB0");
extern u32 pad_pressed_word __asm__("D_0013CAE4");        /* controller D_0013C940 + 0x1A4 */
/* 0x221ab8 (update of the confirm prompt): unless prompt_mode is 0x10 or 1 the page goes back at
   once; otherwise Circle (0x20) clears menu_system +0xD4, goes back and sets the page's answer
   (+0x84) to 1, Triangle (0x10) goes back with the answer 0. Returns 0. */
s32 update_confirm_prompt(u8 *screen) __asm__("FUN_00221ab8");
s32 update_confirm_prompt(u8 *screen) {
    (void)screen;
    u8 *page = *(u8 **)(menu_system_bytes + 4);
    if (prompt_mode != 0x10 && prompt_mode != 1) { *(void **)(menu_system_bytes + 8) = *(void **)(page + 0x38); return 0; }
    const u32 pressed = pad_pressed_word;
    if (pressed & 0x20) {
        *(s32 *)(menu_system_bytes + 0xD4) = 0;
        *(void **)(menu_system_bytes + 8) = *(void **)(page + 0x38);
        *(s32 *)(page + 0x84) = 1;
    } else if (pressed & 0x10) {
        *(void **)(menu_system_bytes + 8) = *(void **)(page + 0x38);
        *(s32 *)(page + 0x84) = 0;
    }
    return 0;
}

/* For the browser world renderer (world-renderer.mjs): a moby's joint matrices as the game's pose
   code computes them (browser/moby-joint-matrices.c, 64 bytes per joint: rows 0-2 rotation and
   scale, row 3 translation, raw model units), into spike_moby_matrix_buffer. Returns the joint
   count (0: none, or more than fit). Host-only: no recovered code calls it. */
static u8 moby_matrix_buffer[64 * 128] __attribute__((aligned(16)));
u8 *spike_moby_matrix_buffer(void) { return moby_matrix_buffer; }
s32 spike_moby_matrices(u8 *moby) { return rnc_moby_all_joint_matrices(moby, moby_matrix_buffer, 128); }

/* Level 0's 0x1ff378 divide_vector_xyz(out, in, d): vdiv Q = 1 / d, out.xyz = in.xyz * Q, out.w =
   in.w (the whole quadword is stored). Callers declare (vec, vec, float) or (float, vec, vec); on
   the EE both load the same registers (a0, a1, f12), and normalize-sources.py adapts the second. */
void divide_vector_xyz(f32 *out, const f32 *in, f32 d) __asm__("FUN_L00_001ff378");
void divide_vector_xyz(f32 *out, const f32 *in, f32 d) {
    const f32 q = 1.0f / d;
    f32 r[4];
    __builtin_memcpy(r, in, 16);
    for (s32 k = 0; k < 3; k++) r[k] *= q;
    __builtin_memcpy(out, r, 16);
}

/* Level 0's small maths helpers (no recovered C), each read from its listing. Vectors are
   quadwords; "xyz" means the w lane is kept from the first operand, as the VU masks do. */
static void vload(f32 *v, const void *p) { __builtin_memcpy(v, p, 16); }
static void vstore(void *p, const f32 *v) { __builtin_memcpy(p, v, 16); }
/* 0x1fefc8: a countdown byte. 0 stays 0 and returns 1; otherwise it is decremented and stored,
   returning 0 while it is still positive and 2 when it reaches 0. */
s32 countdown_byte(u8 *counter) __asm__("FUN_L00_001fefc8");
s32 countdown_byte(u8 *counter) {
    if (*counter == 0) return 1;
    *counter = (u8)(*counter - 1);
    return *counter > 0 ? 0 : 2;
}
/* 0x1ff290 / 0x1ff2c0 / 0x1ff318: in place on the second argument (the first is not read):
   a.xyz += b.xyz, a.xyz -= b.xyz, a.xyz *= s. */
void vec_add_in_place(void *unused, f32 *a, const f32 *b) __asm__("FUN_L00_001ff290");
void vec_add_in_place(void *unused, f32 *a, const f32 *b) { (void)unused; f32 v[4], w[4]; vload(v, a); vload(w, b); for (s32 k = 0; k < 3; k++) v[k] += w[k]; vstore(a, v); }
void vec_sub_in_place(void *unused, f32 *a, const f32 *b) __asm__("FUN_L00_001ff2c0");
void vec_sub_in_place(void *unused, f32 *a, const f32 *b) { (void)unused; f32 v[4], w[4]; vload(v, a); vload(w, b); for (s32 k = 0; k < 3; k++) v[k] -= w[k]; vstore(a, v); }
void vec_scale_in_place(void *unused, f32 *a, f32 s) __asm__("FUN_L00_001ff318");
void vec_scale_in_place(void *unused, f32 *a, f32 s) { (void)unused; f32 v[4]; vload(v, a); for (s32 k = 0; k < 3; k++) v[k] *= s; vstore(a, v); }
/* 0x1ff348: out = a with xyz *= b.xyz. */
void vec_mul_xyz(f32 *out, const f32 *a, const f32 *b) __asm__("FUN_L00_001ff348");
void vec_mul_xyz(f32 *out, const f32 *a, const f32 *b) { f32 v[4], w[4]; vload(v, a); vload(w, b); for (s32 k = 0; k < 3; k++) v[k] *= w[k]; vstore(out, v); }
/* 0x1ff430: (x*x + y*y) + z*z. */
f32 vec_length2(const f32 *a) __asm__("FUN_L00_001ff430");
f32 vec_length2(const f32 *a) { f32 v[4]; vload(v, a); return (v[0] * v[0] + v[1] * v[1]) + v[2] * v[2]; }
/* 0x1ff600 (limit, out, in): when the xy length of in is nonzero and not below limit, out = in with
   xy scaled to the limit and 1 is returned; otherwise out is untouched and 0 is returned. */
s32 clamp_xy_length(f32 limit, f32 *out, const f32 *in) __asm__("FUN_L00_001ff600");
s32 clamp_xy_length(f32 limit, f32 *out, const f32 *in) {
    f32 v[4];
    vload(v, in);
    const f32 length = __builtin_sqrtf(v[0] * v[0] + v[1] * v[1]);
    const f32 slack = limit - length;
    u32 length_bits, slack_bits;
    __builtin_memcpy(&length_bits, &length, 4);
    __builtin_memcpy(&slack_bits, &slack, 4);
    if (length_bits == 0 || (s32)slack_bits > 0) return 0;
    const f32 q = limit / length;
    v[0] *= q; v[1] *= q;
    vstore(out, v);
    return 1;
}
/* 0x1ff660 reflect_vector(out, a, n): d = (-a) . normalize(n); when d is negative (sign bit) out =
   a, otherwise out.xyz = -a + 2 * (normalize(n) * d - (-a)), w from a. */
void reflect_vector_l00(f32 *out, const f32 *a, const f32 *n) __asm__("FUN_L00_001ff660");
void reflect_vector_l00(f32 *out, const f32 *a, const f32 *n) {
    f32 v1[4], v2[4];
    vload(v1, a); vload(v2, n);
    for (s32 k = 0; k < 3; k++) v1[k] = -v1[k];
    const f32 inv = 1.0f / __builtin_sqrtf((v2[0] * v2[0] + v2[1] * v2[1]) + v2[2] * v2[2]);
    for (s32 k = 0; k < 3; k++) v2[k] *= inv;
    const f32 d = (v1[0] * v2[0] + v1[1] * v2[1]) + v1[2] * v2[2];
    if (__builtin_signbit(d)) { __builtin_memcpy(out, a, 16); return; }
    for (s32 k = 0; k < 3; k++) { v2[k] = v2[k] * d - v1[k]; v2[k] += v2[k]; v1[k] += v2[k]; }
    vstore(out, v1);
}
/* 0x1ff768: the four lanes of v * s as integers (vftoi0), clamped to s16, packed into 64 bits. */
u64 pack_vector_s16(const f32 *v, f32 s) __asm__("FUN_L00_001ff768");
u64 pack_vector_s16(const f32 *v, f32 s) {
    f32 a[4];
    vload(a, v);
    u64 out = 0;
    for (s32 k = 0; k < 4; k++) {
        s32 x = ee_ftoi(a[k] * s);
        x = x > 0x7fff ? 0x7fff : x < -0x8000 ? -0x8000 : x;
        out |= (u64)(u16)x << (16 * k);
    }
    return out;
}
/* 0x1ffd48: out matrix = in matrix * s (all 16 lanes). 0x1ffe20: out matrix = a + b. */
void matrix_scale_l00(f32 *out, const f32 *in, f32 s) __asm__("FUN_L00_001ffd48");
void matrix_scale_l00(f32 *out, const f32 *in, f32 s) { f32 m[16]; __builtin_memcpy(m, in, 64); for (s32 k = 0; k < 16; k++) m[k] *= s; __builtin_memcpy(out, m, 64); }
void matrix_add_l00(f32 *out, const f32 *a, const f32 *b) __asm__("FUN_L00_001ffe20");
void matrix_add_l00(f32 *out, const f32 *a, const f32 *b) { f32 m[16], n[16]; __builtin_memcpy(m, a, 64); __builtin_memcpy(n, b, 64); for (s32 k = 0; k < 16; k++) m[k] += n[k]; __builtin_memcpy(out, m, 64); }
/* 0x200228 (whole, x): *whole = (float)(int)x (cvt.w.s truncates), returns x - *whole. */
f32 split_float_fraction(f32 *whole, f32 x) __asm__("FUN_L00_00200228");
f32 split_float_fraction(f32 *whole, f32 x) { const f32 w = (f32)ee_ftoi(x); *whole = w; return x - w; }
/* 0x200240 / 0x200250: cvt.s.w and cvt.w.s (truncating). */
f32 int_to_float_l00(s32 x) __asm__("FUN_L00_00200240");
f32 int_to_float_l00(s32 x) { return (f32)x; }
s32 float_to_int_l00(f32 x) __asm__("FUN_L00_00200250");
s32 float_to_int_l00(f32 x) { return ee_ftoi(x); }
/* 0x200260 (a, b): q = a / b; returns (q - trunc(q)) * b (vftoi0 truncates). */
f32 float_remainder_l00(f32 a, f32 b) __asm__("FUN_L00_00200260");
f32 float_remainder_l00(f32 a, f32 b) { const f32 q = a / b; return (q - (f32)ee_ftoi(q)) * b; }

/* sprintf (0x116248): the recovered newlib sprintf (src/sdk/libc/stdio/sprintf.c) walks the EE's
   register-save area for its arguments (__builtin_next_arg(format) - 0x30), which WebAssembly does
   not have; this formats with the host libc's vsprintf instead (the game uses plain %d / %s /
   %x conversions). */
#include <stdarg.h>
extern int vsprintf(char *out, const char *format, va_list arguments);
int game_sprintf(char *out, const char *format, ...) __asm__("FUN_00116248");
int game_sprintf(char *out, const char *format, ...) {
    va_list arguments;
    va_start(arguments, format);
    const int n = vsprintf(out, format, arguments);
    va_end(arguments);
    return n;
}

/* Level 0x254b58 (2664 bytes, VU0 assembly, no boot copy; the next function after the level's
   copy of 0x2109b8), called by pack_pose (hero_idle_sequence.c) as (moby): the moby's local
   pose for every joint, left in the scratchpad as 64-byte slots { rotation, scale + flag word,
   translation + parent word, ... }, which pack_pose then reads at 0x70000000 + 64 * joint.
   Its opening matches 0x2109b8's (class joint records DMA'd to the scratchpad, current/next
   frame, blend time) without the joint-flag argument, so it is taken as 0x2109b8's local stage
   for all joints (rnc_moby_local_pose, as park_moby_pose above); ASSUMED, not decoded past the
   first 30 instructions. The slots are copied to the spike's scratchpad (0xF0000). */
void evaluate_moby_local_pose_all(u8 *moby) __asm__("FUN_L00_00254b58");
u32 spike_local_pose_calls; /* how often the game reached it (exported for the run report) */
void evaluate_moby_local_pose_all(u8 *moby) {
    spike_local_pose_calls++;
    const u8 *pclass = *(u8 **)(moby + 0x24);
    const s32 joints = pclass[8];
    u8 *scratchpad = (u8 *)0xF0000;
    if (joints <= 0) return;
    rnc_moby_local_pose(moby);
    for (s32 j = 0; j < joints && j < 0x7f; j++) __builtin_memcpy(scratchpad + 64 * j, rnc_moby_pose_slot(j), 64);
}


/* Level 0x252990 (no C, level code only): the animation layer's pose (moby-joint-matrices.c). */
void evaluate_animation_layer_pose(u8 *state) __asm__("FUN_L00_00252990");
void evaluate_animation_layer_pose(u8 *state) { rnc_animation_layer_pose(state); }

/* Level 0x250320 (12 instructions, level code only): get_moby_color(moby, &r, &g, &b), the bytes
   4, 5 and 6 of the 64-bit word at moby + 0x38 (dsrl32 0 / 8 / 16, andi 0xff). spawn_explosion. */
s32 get_moby_color_l00(const u8 *moby, s32 *r, s32 *g, s32 *b) __asm__("FUN_L00_00250320");
s32 get_moby_color_l00(const u8 *moby, s32 *r, s32 *g, s32 *b) {
    *r = moby[0x38 + 4];
    *g = moby[0x38 + 5];
    *b = moby[0x38 + 6];
    return 0; /* v0 is not set; the caller's declaration reads one */
}

/* Level 0x250430 (19 instructions, level code only), weapon_ammo's jet_distance(a, b): the gap
   between two spheres, (|a.xyz - b.xyz| - (a.w + b.w)) / 1024 (vsqrt of the summed squares; vf3.x
   = 0x3a800000 = 1/1024 scales both terms; the result is returned in f0). */
f32 jet_distance_l00(const f32 *a, const f32 *b) __asm__("FUN_L00_00250430");
f32 jet_distance_l00(const f32 *a, const f32 *b) {
    const f32 dx = a[0] - b[0], dy = a[1] - b[1], dz = a[2] - b[2];
    const f32 k = 1.0f / 1024.0f;
    return k * __builtin_sqrtf(dx * dx + dy * dy + dz * dz) - k * (a[3] + b[3]);
}
