/* Moby colour (asm): sets and gets the colour packed in the 64-bit word at moby+0x38. */
#include "types.h"
#include "asm.h"

/* Packs red, green and blue into the upper bytes of the 64-bit word at moby+0x38. */
/* Intentional asm: hand-written in retail (explicit nop before jr $31, a slot
 * scheduling the compiler never leaves empty); rac1-decomp also keeps it as asm. */
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002502f0.s", FUN_L00_002502f0);
/* Intentional asm: hand-written in retail ($at used as a general register,
 * which the compiler never allocates); rac1-decomp also keeps it as asm. */
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_00250320.s", FUN_L00_00250320);
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_00250430.s", FUN_L00_00250430);
