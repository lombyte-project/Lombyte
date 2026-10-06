#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/math/multiply_global_factor_ed70/func_001F9730.s",
            func_001F9730);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json), so it has no C goal and no public
   fuzzy score. The assembly oracle above is the whole unit. It belongs to
   Insomniac's hand-written fast_* math module: addi, $at and $f14/$f15 as
   scratch registers and no hazard nop after mtc1, none of which the
   compiler emits. */
#endif /* NON_MATCHING */
