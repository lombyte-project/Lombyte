#ifndef LOMBYTE_RNC_MATH_CIRCLE_OFFSET_H
#define LOMBYTE_RNC_MATH_CIRCLE_OFFSET_H

#include "types.h"
#include "rnc/math/vector.h"

f32 fast_cos(f32) __asm__("FUN_001f9dc8");
f32 fast_sin(f32) __asm__("FUN_001f9de0");

/* Moves v's x/y by 2 * r toward angle ang. Kept an inline function (not a
   macro): the ledge probes FUN_L00_0020c758 / FUN_L01_0022d838 match only so. */
static inline void circle_offset(Vec4 *v, f32 ang, f32 r)
{
    r += r;
    v->f[0] += fast_cos(ang) * r;
    v->f[1] += fast_sin(ang) * r;
}

#endif /* LOMBYTE_RNC_MATH_CIRCLE_OFFSET_H */
