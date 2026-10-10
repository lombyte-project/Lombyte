#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM(
    "config/us/expected/asm/assembly/runtime/memory/fill_transfer_words/FillTransferWords.s",
    FillTransferWords);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json), so it has no C goal and no public
   fuzzy score. The assembly oracle above is the whole unit. It steps with
   `addi`, the trapping add, where the compiler always emits `addiu`. */
#endif /* NON_MATCHING */
