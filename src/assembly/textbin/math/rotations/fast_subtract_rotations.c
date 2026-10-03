#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/math/rotations/fast_subtract_rotations/FUN_001fa5c8.s", FUN_001fa5c8);
#else
#include "types.h"
#define PI 3.1415927f
f32 fast_subtract_rotations(f32 angle, f32 delta) __asm__("FUN_001fa5c8");
f32 fast_subtract_rotations(f32 angle, f32 delta) {
    f32 difference;
    difference = angle - delta;
    if (!(difference < PI)) {
        difference = (difference - PI) - PI;
    }
    if (difference < -PI) {
        difference = (difference + PI) + PI;
    }
    return difference;
}

extern __typeof__(fast_subtract_rotations) func_001FA5C8 __attribute__((alias("FUN_001fa5c8")));

#endif /* NON_MATCHING */
