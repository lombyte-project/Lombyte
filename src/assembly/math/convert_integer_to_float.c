#include "types.h"
#include "asm.h"

/* Has no hazard nop between mtc1 and cvt.s.w, which the assembler inserts
   after every compiled mtc1. */
INCLUDE_ASM("config/us/expected/asm/assembly/math/convert_integer_to_float/func_001FA6C0.s", convert_integer_to_float);
