#ifndef LOMBYTE_RNC_MATH_SPHERICAL_OFFSET_H
#define LOMBYTE_RNC_MATH_SPHERICAL_OFFSET_H

#include "types.h"

/* An offset of length scale at the two given angles. */
void build_spherical_offset(void *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");

#endif /* LOMBYTE_RNC_MATH_SPHERICAL_OFFSET_H */
