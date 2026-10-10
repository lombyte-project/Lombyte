#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM(
    "config/us/expected/asm/assembly/textbin/math/rotations/fast_normalize_angle/FUN_001fa610.s",
    FUN_001fa610);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json), like its fastfunc neighbours
   fast_add_rotations and fast_subtract_rotations. Retail builds the two
   pi constants with $at as a general register (lui $1 / ori $1, mtc1 $1)
   interleaved with $2, and puts two nops between each c.lt.s and its
   bc1t/bc1f; the compiler emits neither. The plain C loop scores 24/30. */
#endif /* NON_MATCHING */
