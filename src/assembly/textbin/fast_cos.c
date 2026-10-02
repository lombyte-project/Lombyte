#include "types.h"
#include "asm.h"

f32 fast_cos(f32 radians) __asm__("FUN_001f9dc8");

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fast_cos/FUN_001f9dc8.s", FUN_001f9dc8);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json), so it has no C goal and no public
   fuzzy score. The assembly oracle above is the whole unit. */
#endif /* NON_MATCHING */
