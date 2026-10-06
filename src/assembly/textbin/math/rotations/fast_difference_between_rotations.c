#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/math/rotations/"
            "fast_difference_between_rotations/FUN_001fa688.s",
            FUN_001fa688);
#else
#include "types.h"
f32 fast_difference_between_rotations(f32 first_angle, f32 second_angle) __asm__("FUN_001fa688");
f32 fast_difference_between_rotations(f32 first_angle, f32 second_angle) {
    f32 pi_values[1] = {(f32)3.141592653589793};
    f32 difference = first_angle - second_angle;
    f32 pi = pi_values[0];
    difference = fabsf(difference);
    if (!(difference < pi)) {
        pi = pi + pi;
        difference = pi - difference;
    }
    return difference;
}

extern __typeof__(fast_difference_between_rotations) func_001FA688
    __attribute__((alias("FUN_001fa688")));

#endif /* NON_MATCHING */
