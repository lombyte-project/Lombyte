#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/compute_tfrag_texture_usage/FUN_00234bd8.s",
            FUN_00234bd8);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json), so it has no C goal and no public
   fuzzy score. The assembly oracle above is the whole unit. */
#endif /* NON_MATCHING */
