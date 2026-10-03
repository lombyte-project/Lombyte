#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/math/rotations/fast_add_rotations/FUN_001fa580.s", FUN_001fa580);
#else
#include "types.h"
f32 fast_add_rotations(f32 angle, f32 delta) __asm__("FUN_001fa580");

f32 fast_add_rotations(f32 angle, f32 delta) {
    f32 wrapped_angle;
    f32 sum_angle;
    sum_angle = angle + delta;
    wrapped_angle = sum_angle;
    if (!(wrapped_angle < 3.1415927f)) {
        wrapped_angle = wrapped_angle - 3.1415927f - 3.1415927f;
    }
    if (sum_angle < -3.1415927f) {
        wrapped_angle = wrapped_angle + 3.1415927f + 3.1415927f;
    }
    return wrapped_angle;
}

extern __typeof__(fast_add_rotations) func_001FA580 __attribute__((alias("FUN_001fa580")));

#endif /* NON_MATCHING */
