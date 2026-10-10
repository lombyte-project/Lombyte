#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM(
    "config/us/expected/asm/assembly/textbin/math/rotations/fast_normalize_angle/FUN_001fa610.s",
    FUN_001fa610);
#else
/* No C body on purpose: this unit is intentional low-level assembly
   (config/us/unit_categories.json), like its fastfunc neighbours
   fast_add_rotations and fast_subtract_rotations. Retail interleaves two
   constant builds: lui $1 / lui $2 / ori $1,$1 / ori $2,$2. $at is never
   allocated by gcc, so $1 can only come from the assembler's li.s macro,
   and a macro expansion is always contiguous. Compiling the C loop and
   assembling it with ps2eeas gives exactly that contiguous
   lui $1 / ori $1 / mtc1 $1 pair, twice, at BYTES 24/30. The nops agree:
   ps2eeas puts one hazard nop between c.lt.s and bc1x, while retail has
   two after the loop tests and none (an mtc1 instead) after the first. */
#endif /* NON_MATCHING */
