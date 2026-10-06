#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM(
    "config/us/expected/asm/assembly/textbin/math/rotations/fast_subtract_rotations/FUN_001fa5c8.s",
    FUN_001fa5c8);
#else
#include "types.h"
#define PI 3.1415927f
f32 fast_subtract_rotations(f32 angle, f32 delta) __asm__("FUN_001fa5c8");
f32 fast_subtract_rotations(f32 angle, f32 delta) {
    f32 pi = PI;
    f32 difference = angle - delta;
    f32 sum = difference + pi;
    f32 wrapped_angle = difference;
    if (!(difference < pi)) {
        wrapped_angle = (wrapped_angle - pi) - pi;
    }
    if (sum < 0.0f) {
        wrapped_angle = sum + pi;
    }
    return wrapped_angle;
}

extern __typeof__(fast_subtract_rotations) func_001FA5C8 __attribute__((alias("FUN_001fa5c8")));

#endif /* NON_MATCHING */
