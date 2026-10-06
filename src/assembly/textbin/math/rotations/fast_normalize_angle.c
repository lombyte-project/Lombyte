#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM(
    "config/us/expected/asm/assembly/textbin/math/rotations/fast_normalize_angle/FUN_001fa610.s",
    FUN_001fa610);
#else
#include "types.h"
#define PI 3.1415927f
f32 fast_normalize_angle(f32 angle) __asm__("FUN_001fa610");
f32 fast_normalize_angle(f32 angle) {
    f32 wrapped_angle;
    f32 pi = PI;
    f32 negative_pi = -PI;
    wrapped_angle = angle;
    do {
        if (wrapped_angle < pi)
            break;
        wrapped_angle = (wrapped_angle - pi) - pi;
    } while (!(wrapped_angle < pi));
    do {
        if (!(wrapped_angle < negative_pi))
            break;
        wrapped_angle = (wrapped_angle + pi) + pi;
    } while (wrapped_angle < negative_pi);
    return wrapped_angle;
}

extern __typeof__(fast_normalize_angle) func_001FA610 __attribute__((alias("FUN_001fa610")));

#endif /* NON_MATCHING */
