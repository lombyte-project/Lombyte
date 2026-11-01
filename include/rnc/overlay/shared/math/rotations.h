#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MATH_ROTATIONS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MATH_ROTATIONS_H

#include "types.h"

void rotate_vector_by_scaled_angle_between_vectors(float, float *, float *, float *, int) __asm__("FUN_L03_0024e6f8");
float step_rotation_toward_target(float, float, float) __asm__("FUN_L00_00200510");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MATH_ROTATIONS_H */
