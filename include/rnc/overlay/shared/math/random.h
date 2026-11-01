#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MATH_RANDOM_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MATH_RANDOM_H

#include "types.h"

void  generate_random_scaled_vector2(float *, float) __asm__("FUN_L09_00277758");
float scale_by_one_minus_random_squared(float) __asm__("FUN_L09_00277688");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MATH_RANDOM_H */
