#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/math/rotations/"
            "fast_difference_between_rotations/FUN_001fa688.s",
            FUN_001fa688);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json), so it has no C goal and no public
   fuzzy score. The assembly oracle above is the whole unit. It has a
   hand-placed nop before abs.s, which does not read $f14, and adds pi to
   itself at run time where the compiler folds the constant. */
#endif /* NON_MATCHING */
