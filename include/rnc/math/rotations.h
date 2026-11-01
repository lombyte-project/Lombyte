#ifndef LOMBYTE_RNC_MATH_ROTATIONS_H
#define LOMBYTE_RNC_MATH_ROTATIONS_H

#include "types.h"

struct Quaternion;

void build_quaternion_from_axis_angle(struct Quaternion *, s32, f32) __asm__("FUN_00214530");

#endif /* LOMBYTE_RNC_MATH_ROTATIONS_H */
