#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fast_decompress/FUN_0020b618.s", FUN_0020b618);
#else
/* No C body on purpose: retail is hand-written (no stack frame, pcpyld ra
   and qmtc2.ni zero, vf0), so this unit is intentional low-level assembly
   (config/us/unit_categories.json). */
#endif /* NON_MATCHING */
