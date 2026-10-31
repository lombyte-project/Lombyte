#include "types.h"
#include "asm.h"

/* Has a hand-placed nop before abs.s, which does not read $f14, and adds pi
   to itself at run time where the compiler folds the constant. */
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/math/rotations/"
            "fast_difference_between_rotations/FUN_001fa688.s", fast_difference_between_rotations);
