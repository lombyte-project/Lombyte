#include "types.h"
#include "asm.h"

/* Steps with `addi`, the trapping add, where the compiler always emits
   `addiu`. */
INCLUDE_ASM("config/us/expected/asm/assembly/runtime/memory/fill_transfer_words/FillTransferWords.s", FillTransferWords);
