#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MATH_VU0_VECTOR_OPS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MATH_VU0_VECTOR_OPS_H

#include "types.h"

s32  fast_subtract_rotations_cf(void *) __asm__("FUN_L00_001fefc8");
void add_vectors(void *, void *, void *) __asm__("FUN_L00_001ff290");
void reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
void rotation_2D5FA8(void *, s32, f32) __asm__("FUN_L00_001fff28");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MATH_VU0_VECTOR_OPS_H */
