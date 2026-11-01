#include "types.h"
#include "asm.h"

/* Part of Insomniac's hand-written fast_* math module: addi, $at and
   $f14/$f15 as scratch registers and no hazard nop after mtc1, none of which
   the compiler emits. */
INCLUDE_ASM("config/us/expected/asm/assembly/math/conversion/truncate_float_to_s32/FUN_001fa6d0.s", FUN_001fa6d0);
