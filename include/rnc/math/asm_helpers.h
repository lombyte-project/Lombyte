#ifndef LOMBYTE_RNC_MATH_ASM_HELPERS_H
#define LOMBYTE_RNC_MATH_ASM_HELPERS_H

#include "types.h"

/* The other assembly helpers (src/assembly/textbin) that game and level
   code call. Files whose local prototypes differ keep them. */
s32  truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
f32  fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
f32  fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
s32  fast_dec_timer(void *) __asm__("FUN_001f9740");
f32  vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
s32  fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
void clear_u64_value(void *) __asm__("FUN_001f99f8");
f32  fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
f32  vector_length_xy(void *) __asm__("FUN_001f9b20");
f32  fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
f32  convert_integer_to_float(s32) __asm__("func_001FA6C0");
void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
void set_vector_length_xy(void *, void *, f32) __asm__("FUN_001f9c48");
void transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
f32  square_root(f32) __asm__("FUN_001f9988");
void lerp_vector(void *, void *, void *, f32) __asm__("FUN_001f9a40");
s32  fast_bsphere_check(void *, f32) __asm__("FUN_001fa728");
void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
void set_identity_matrix(void *) __asm__("FUN_001f9fc8");
void euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");
void copy_matrix3x4(void *, void *) __asm__("FUN_001fa2b8");
void fast_mem_zero16(void *, s32) __asm__("FUN_001f9810");
void clamp_vector_length(void *, void *, f32) __asm__("FUN_001f9c90");
void copy_matrix_rotation(void *, void *) __asm__("FUN_001fa298");
f32  fast_arc_sin(f32) __asm__("FUN_001f9df8");
f32  fast_normalize_angle(f32) __asm__("FUN_001fa610");
void euler_to_matrix_ee(void *, void *) __asm__("FUN_001fa070");
void multiply_quaternions(void *, void *, void *) __asm__("FUN_001fa3c0");
void copy_memory_aligned(void *, void *, s32) __asm__("FUN_001f9838");
void quaternion_nlerp(void *, void *, void *, f32) __asm__("FUN_001fa400");
void fast_mem_copy(void *, void *, s32) __asm__("FUN_001f98d0");
void set_scale_matrix(void *, f32) __asm__("FUN_001f9ff8");
void multiply_matrix3x3(void *, void *, void *) __asm__("FUN_001fa328");
void quaternion_to_rotation_basis(void *, void *) __asm__("FUN_001fa480");
void set_vector_unit_z(void *) __asm__("FUN_001f9a00");
f32  max_float(f32, f32) __asm__("FUN_001f99c8");
u32  is_vector_outside_clip(void *) __asm__("FUN_001f9958");
f32  min_float(f32, f32) __asm__("FUN_001f99d0");

#endif /* LOMBYTE_RNC_MATH_ASM_HELPERS_H */
