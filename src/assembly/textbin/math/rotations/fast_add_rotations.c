#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM(
    "config/us/expected/asm/assembly/textbin/math/rotations/fast_add_rotations/FUN_001fa580.s",
    FUN_001fa580);
#else
#include "types.h"
f32 fast_add_rotations(f32 angle, f32 delta) __asm__("FUN_001fa580");

f32 fast_add_rotations(f32 angle, f32 delta) {
    f32 wrapped_angle = angle + delta;
    f32 pi = 3.1415927f;
    f32 negative_pi = -pi;
    if (!(wrapped_angle < pi)) {
        wrapped_angle = wrapped_angle - pi;
        wrapped_angle = wrapped_angle - pi;
    }
    if (wrapped_angle < negative_pi) {
        wrapped_angle = wrapped_angle + pi;
        wrapped_angle = wrapped_angle + pi;
    }
    return wrapped_angle;
}
extern __typeof__(fast_add_rotations) func_001FA580 __attribute__((alias("FUN_001fa580")));

#endif /* NON_MATCHING */
