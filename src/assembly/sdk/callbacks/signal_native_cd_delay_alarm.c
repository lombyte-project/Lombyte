#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM(
    "config/us/expected/asm/assembly/sdk/callbacks/signal_native_cd_delay_alarm/FUN_001205e8.s",
    FUN_001205e8);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json), so it has no C goal and no public
   fuzzy score. The assembly oracle above is the whole unit. */
#endif /* NON_MATCHING */
