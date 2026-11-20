/* Portable versions of boot-ELF helpers that exist only as EE assembly
   (src/assembly/textbin). Each one follows the instructions at its address;
   the comments name the instructions. Results use IEEE single precision. The
   EE FPU and VU0 flush denormals and round toward zero, so last-bit
   differences from the console are possible; no caller here depends on them.

   Both label spellings are defined where recovered sources use both
   (FUN_001fa580 in asm_helpers.h, func_001FA580 in sample_camera_path.c). */
#include "types.h"

#define ALIAS(type, name, label, target) \
    extern __typeof__(name) name##_##label __asm__(#label) __attribute__((alias(#target)));

typedef struct {
    f32 x, y, z, w;
} LevelVec4;

/* Callers pass their own vector types; copy instead of type-punning so LTO's
   alias analysis cannot reorder the accesses. */
static LevelVec4 load_vec(const void *from) {
    LevelVec4 v;
    __builtin_memcpy(&v, from, sizeof(v));
    return v;
}

/* 0x1fa580: add.s; if !(r < pi) r = (r - pi) - pi; else if (r < -pi)
   r = (r + pi) + pi. The second compare sits in the branch delay slot, so it
   tests the sum before any correction. */
static f32 wrap_rotation(f32 r) {
    const f32 pi = 3.14159274f; /* 0x40490fdb */
    if (!(r < pi)) {
        r = r - pi;
        return r - pi;
    }
    if (r < -pi) {
        r = r + pi;
        r = r + pi;
    }
    return r;
}

f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
f32 fast_add_rotations(f32 a, f32 b) { return wrap_rotation(a + b); }
ALIAS(fast_add_rotations, fast_add_rotations, func_001FA580, FUN_001fa580)

/* 0x1fa5c8: the same with sub.s. */
f32 fast_subtract_rotations(f32 a, f32 b) __asm__("FUN_001fa5c8");
f32 fast_subtract_rotations(f32 a, f32 b) { return wrap_rotation(a - b); }
ALIAS(fast_subtract_rotations, fast_subtract_rotations, func_001FA5C8, FUN_001fa5c8)

/* 0x1fa6d0: cvt.w.s; mfc1. The EE saturates out-of-range values (it has no
   NaN encoding); wasm's i32.trunc_f32_s would trap, so clamp first. */
s32 truncate_float_to_s32(f32 x) __asm__("FUN_001fa6d0");
s32 truncate_float_to_s32(f32 x) {
    if (x != x || x >= 2147483648.0f) {
        return 0x7FFFFFFF;
    }
    if (x <= -2147483648.0f) {
        return (s32)0x80000000;
    }
    return (s32)x;
}
ALIAS(truncate_float_to_s32, truncate_float_to_s32, func_001FA6D0, FUN_001fa6d0)

/* 0x1fa6c0: mtc1; cvt.s.w. */
f32 convert_integer_to_float(s32 i) __asm__("FUN_001fa6c0");
f32 convert_integer_to_float(s32 i) { return (f32)i; }
ALIAS(convert_integer_to_float, convert_integer_to_float, func_001FA6C0, FUN_001fa6c0)

/* 0x1f9a28: lqc2 a, b; vsub.xyz out = a - b (w keeps a.w); sqc2. */
void fast_vec_sub(void *out, void *a, void *b) __asm__("FUN_001f9a28");
void fast_vec_sub(void *out, void *a, void *b) {
    const LevelVec4 va = load_vec(a);
    const LevelVec4 vb = load_vec(b);
    LevelVec4 r = { va.x - vb.x, va.y - vb.y, va.z - vb.z, va.w };
    __builtin_memcpy(out, &r, sizeof(r));
}

/* 0x1f9a40: t in $f12; out.xyz = a + (b - a) * t, out.w = a.w. */
void lerp_vector(void *out, void *a, void *b, f32 t) __asm__("FUN_001f9a40");
void lerp_vector(void *out, void *a, void *b, f32 t) {
    const LevelVec4 va = load_vec(a);
    const LevelVec4 vb = load_vec(b);
    LevelVec4 r = { va.x + (vb.x - va.x) * t, va.y + (vb.y - va.y) * t, va.z + (vb.z - va.z) * t,
                    va.w };
    __builtin_memcpy(out, &r, sizeof(r));
}

/* 0x1f9b20: vmul.xyz; vaddy.x; vsqrt; vaddq.x -> sqrt(x*x + y*y). */
f32 vector_length_xy(void *v) __asm__("FUN_001f9b20");
f32 vector_length_xy(void *v) {
    const LevelVec4 p = load_vec(v);
    return __builtin_sqrtf(p.x * p.x + p.y * p.y);
}

/* 0x1f9b48: vsub.xyz; vmul.xyz; vaddabc.x (x + y); vmaddbc.x (+ 1 * z);
   vsqrt -> distance over x, y, z. */
f32 fast_vec_dist(void *a, void *b) __asm__("FUN_001f9b48");
f32 fast_vec_dist(void *a, void *b) {
    const LevelVec4 va = load_vec(a);
    const LevelVec4 vb = load_vec(b);
    f32 dx = va.x - vb.x;
    f32 dy = va.y - vb.y;
    f32 dz = va.z - vb.z;
    return __builtin_sqrtf((dx * dx + dy * dy) + dz * dz);
}

/* 0x1f9e90: atan2-style angle of (x, y) via atan(t) + pi/4 with
   t = (a - b) / (a + b), where a and b are |x| and |y| ordered so |t| <= 1:
     abs.s f1 = |x|, f2 = |y|; v1 = bits(f1 - f2)
     if v1 >= 0: f1 = |y|, f2 = |x|   (bltz skips the swap)
     if f2 <= 0: return 0             (bc1tl to 0x1f9f98)
     t = (f1 - f2) / (f1 + f2)
   The octant row at 0x1de340 is picked by sign bits:
   (bits(|x| - |y|) >> 31) * 8 + (bits(y) >> 31) * 16 + (bits(x) >> 31) * 32,
   and the result is (pi/4 + poly(t)) * row[0] + row[1]. poly is the odd
   series t..t^15 with the coefficients at 0x1de320, summed in VU0 ACC order. */
static const f32 atan_coefficients[8] = {
    0.999999344f, -0.333298564f, 0.199465364f, -0.139085338f,
    0.0964200422f, -0.0559098870f, 0.0218612291f, -0.00405405788f};
static const f32 atan_octants[16] = {
    1.0f, 0.0f, -1.0f, 1.57079637f, -1.0f, 0.0f, 1.0f, -1.57079637f,
    -1.0f, 3.14159274f, 1.0f, 1.57079637f, 1.0f, -3.14159274f, -1.0f, -1.57079637f};

static u32 float_bits(f32 value) {
    union { f32 f; u32 u; } bits = { value };
    return bits.u;
}

f32 fast_arc_tan(f32 x, f32 y) __asm__("FUN_001f9e90");
f32 fast_arc_tan(f32 x, f32 y) {
    f32 ax = __builtin_fabsf(x);
    f32 ay = __builtin_fabsf(y);
    u32 difference = float_bits(ax - ay);
    f32 a = ax;
    f32 b = ay;
    f32 t, t2, t4, t8;
    f32 l0, l1, l2, l3, h0, h1, h2, h3, sum;
    const f32 *row;

    if (!(difference >> 31)) {
        a = ay;
        b = ax;
    }
    if (b <= 0.0f) {
        return 0.0f;
    }
    t = (a - b) / (a + b);
    row = atan_octants + ((difference >> 31) * 2 + (float_bits(y) >> 31) * 4 + (float_bits(x) >> 31) * 8);
    /* vf1 = (t, t, t, t); vf2 = vf1^2; vf3 = vf2^2; vf1.y *= vf2; vf4 = vf3^2;
       vf1.zw *= vf3; vf1.w *= vf2; vf2 = vf1 * vf4. */
    t2 = t * t;
    t4 = t2 * t2;
    t8 = t4 * t4;
    l0 = t;
    l1 = t * t2;
    l2 = t * t4;
    l3 = (t * t4) * t2;
    h0 = l0 * t8;
    h1 = l1 * t8;
    h2 = l2 * t8;
    h3 = l3 * t8;
    l0 *= atan_coefficients[0];
    l1 *= atan_coefficients[1];
    l2 *= atan_coefficients[2];
    l3 *= atan_coefficients[3];
    h0 *= atan_coefficients[4];
    h1 *= atan_coefficients[5];
    h2 *= atan_coefficients[6];
    h3 *= atan_coefficients[7];
    sum = l0 + l1;
    sum = sum + l2;
    sum = sum + l3;
    sum = sum + h0;
    sum = sum + h1;
    sum = sum + h2;
    sum = sum + h3;
    return (0.785398185f + sum) * row[0] + row[1]; /* 0x3f490fdb */
}
/* The level 0 copy at 0x1ff8b0: the same 66 instructions with its table address at 0x1c2420,
   whose 0x60 bytes equal the boot program's at 0x1de320. */
extern __typeof__(fast_arc_tan) fast_arc_tan_level __asm__("FUN_L00_001ff8b0") __attribute__((alias("FUN_001f9e90")));

/* 0x1160d8, newlib rand: next = next * 0x41c64e6d + 0x3039;
   return next & 0x7fffffff. The seed (_impure_ptr + 0x58) starts at 1; no
   srand call on the boot path has been traced, so 1 is assumed. */
static u32 rand_next = 1;

s32 level_rand(void) __asm__("FUN_001160d8");
s32 level_rand(void) {
    rand_next = rand_next * 0x41C64E6DU + 0x3039U;
    return (s32)(rand_next & 0x7FFFFFFFU);
}

void level_math_reset(void) { rand_next = 1; }
