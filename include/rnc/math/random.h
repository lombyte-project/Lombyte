#ifndef LOMBYTE_RNC_MATH_RANDOM_H
#define LOMBYTE_RNC_MATH_RANDOM_H

#include "types.h"

/* Random numbers from rand(). */
s32 random_integer_below(s32 limit) __asm__("FUN_00213260");
f32 random_float_between(f32 range_start, f32 range_end) __asm__("FUN_002132a8");
f32 random_angle_radians(void) __asm__("FUN_00213308");

#endif /* LOMBYTE_RNC_MATH_RANDOM_H */
