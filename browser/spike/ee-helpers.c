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

/* create_part, level 0 0x2678b8: `j 0x2678c0` with a1 = 0. 0x2678c0, with gp = 0x166c00:
   next = [gp - 0x6a90] (0x160170), highest = [gp - 0x6a8c] (0x160174), table = [gp - 0x6a94]
   (0x16016c), count = [gp - 0x6a88] (0x160178), allocation bits at 0x1b1880 (0x800 particles).
   If next >= 0x800, return 0. Otherwise set next's bit; then if next > highest, next + 1 becomes
   the next free index, else the search starts at the byte holding next + 1 and takes the first
   clear bit (up to 0x800). highest = max(highest, index) (pmaxw, raw word 0x7326c8c8), count + 1.
   The 64-byte particle at table + 64 * index gets its first 32 bytes zeroed (sq 0x0, then 0x10
   three times) and the type in byte 0. The paths at 0x2678e8 and 0x2679c0 are not reached from
   this entry. */
extern s32 particle_next_free __asm__("D_L00_00160170");
extern s32 particle_highest __asm__("D_L00_00160174");
extern u8 *particle_table __asm__("D_L00_0016016C");
extern s32 particle_count __asm__("D_L00_00160178");
extern u8 particle_allocated[0x100] __asm__("D_L00_001B1880");
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
extern u8 *level_moby_table __asm__("D_L00_0015FFD8");
extern f32 level_camera_block[] __asm__("D_L00_00166C80");
extern u16 *level_moby_groups[] __asm__("D_L00_001AB840");
extern u8 *level_flagged_mobys[] __asm__("D_L00_001ABA00");
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
extern u8 *level_update_list_head __asm__("D_L00_0015FFE4");
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
extern f32 hit_default_damage[] __asm__("D_L00_00160080");       /* gp - 0x6b80 */
extern f32 hit_knockback_scale[] __asm__("D_L00_001B01C8");      /* [row][hit type] */
extern u8 hit_reaction_by_kind[] __asm__("D_L00_001B0180");      /* [reaction * 4 + kind] */
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
extern f32 level_view_block[] __asm__("D_L00_0016CCC0");
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
