#include "types.h"
#include "asm.h"

/* Part of Insomniac's hand-written fast_* math module: addi, $at and
   $f14/$f15 as scratch registers and no hazard nop after mtc1, none of which
   the compiler emits. */
INCLUDE_ASM("config/us/expected/asm/assembly/math/multiply_global_factor_ed70/func_001F9730.s", multiply_global_factor_ed70);
