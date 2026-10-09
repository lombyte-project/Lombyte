#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/runtime/memory/clear_u64_value/FUN_001f99f8.s",
            FUN_001f99f8);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json), so it has no C goal and no public
   fuzzy score. The assembly oracle above is the whole unit. It stores
   the zero register straight to memory (`sq $0`); the compiler's quadword
   move has no zero alternative and always goes through `por` first. */
#endif /* NON_MATCHING */
