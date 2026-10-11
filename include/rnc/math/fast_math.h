#ifndef LOMBYTE_RNC_MATH_FAST_MATH_H
#define LOMBYTE_RNC_MATH_FAST_MATH_H

#include "types.h"

/* Boot ELF math helpers (assembly/textbin). Names from the recovered
   symbols (FastVecSub, FastVecDot, FastCos, ...) or from the instructions. */

/* Vector helpers on the x, y and z of 16-byte vectors. */
void fast_vec_add(void *out, void *a, void *b) __asm__("FUN_001f9a10");
void fast_vec_sub(void *out, void *a, void *b) __asm__("FUN_001f9a28");
void fast_vec_scale(void *out, void *a, f32 scale) __asm__("FUN_001f9a68");
f32 fast_vec_dot(void *a, void *b) __asm__("FUN_001f9ab0");
void fast_vec_cross(void *out, void *a, void *b) __asm__("FUN_001f9ad8");
f32 fast_vec_length(void *a) __asm__("FUN_001f9af0");
/* Writes a scaled to the given length. */
void fast_vec_normalize(void *out, void *a, f32 length) __asm__("FUN_001f9bf8");

f32 fast_cos(f32 radians) __asm__("FUN_001f9dc8");
f32 fast_sin(f32 radians) __asm__("FUN_001f9de0");
/* abs.s */
f32 absolute_float(f32 x) __asm__("FUN_001f99c0");
/* cvt.s.w */
f32 convert_integer_to_float(s32 i) __asm__("FUN_001fa6c0");
f32 fast_difference_between_rotations(f32 a, f32 b) __asm__("FUN_001fa688");
/* frames * a global float factor + 0.25, truncated. */
s32 scale_game_frames(s32 frames) __asm__("FUN_001f96f8");

#endif /* LOMBYTE_RNC_MATH_FAST_MATH_H */
