#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/math/rotations/fast_difference_between_rotations/FUN_001fa688.s", FUN_001fa688);
#else
#include "types.h"
f32 fast_difference_between_rotations(f32 first_angle, f32 second_angle) __asm__("FUN_001fa688");
f32 fast_difference_between_rotations(f32 first_angle, f32 second_angle) {
    f32 difference;
    difference = fabsf(first_angle - second_angle);
    /* Retail evaluates (pi + pi) - difference. This draft keeps a different
     * rounding order outside the wrapped input range and remains pending. */
    if (!(difference < 3.1415927f)) {
        difference = (3.1415927f - difference) + 3.1415927f;
    }
    return difference;
}

extern __typeof__(fast_difference_between_rotations) func_001FA688 __attribute__((alias("FUN_001fa688")));

#endif /* NON_MATCHING */
