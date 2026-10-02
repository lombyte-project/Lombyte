#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/math/rotations/fast_difference_between_rotations/FUN_001fa688.s", FUN_001fa688);
#else
#include "types.h"
f32 fast_difference_between_rotations(f32 a, f32 b) __asm__("FUN_001fa688");
f32 fast_difference_between_rotations(f32 a, f32 b) {
    f32 d;
    d = fabsf(a - b);
    if (!(d < 3.1415927f)) {
        d = (3.1415927f - d) + 3.1415927f;
    }
    return d;
}
#endif /* NON_MATCHING */
