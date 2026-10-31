#include "types.h"
#include "asm.h"

/* Retail moves the angle and rows through VU0 (qmtc2, lqc2/sqc2,
   vmulax/vmadday/vmaddaz/vmaddw), which C cannot express. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/sceVu0RotMatrixZ/FUN_001252b8.s", FUN_001252b8);
