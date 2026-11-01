#include "types.h"
#include "asm.h"

/* The second c.lt.s sits in the bc1t delay slot and its flag is reused after
   the subtraction, and -pi is built in $v0; the compiler does neither. */
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/math/rotations/fast_add_rotations/FUN_001fa580.s", fast_add_rotations);
