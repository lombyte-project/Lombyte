#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/sceVu0RotMatrixZ/FUN_001252b8.s",
            FUN_001252b8);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json), so it has no C goal and no public
   fuzzy score. Retail moves the angle and rows through VU0 (qmtc2,
   lqc2/sqc2, vmulax/vmadday/vmaddaz/vmaddw), which C cannot express.
   The assembly oracle above is the whole unit. */
#endif /* NON_MATCHING */
