#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM(
    "config/us/expected/asm/assembly/textbin/math/rotations/fast_subtract_rotations/FUN_001fa5c8.s",
    FUN_001fa5c8);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json), so it has no C goal and no public
   fuzzy score. The assembly oracle above is the whole unit. The second
   c.lt.s sits in the bc1t delay slot and its flag is reused after the
   subtraction, and -pi is built in $v0; the compiler does neither. */
#endif /* NON_MATCHING */
