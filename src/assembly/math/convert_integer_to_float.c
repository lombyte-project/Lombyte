#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/math/convert_integer_to_float/func_001FA6C0.s",
            func_001FA6C0);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json), so it has no C goal and no public
   fuzzy score. The assembly oracle above is the whole unit. It has no
   hazard nop between mtc1 and cvt.s.w, which the assembler inserts after
   every compiled mtc1. */
#endif /* NON_MATCHING */
