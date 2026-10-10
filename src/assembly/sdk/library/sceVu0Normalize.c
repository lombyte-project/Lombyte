#include "types.h"
#include "asm.h"

/* sceVu0Normalize (libvu0): lqc2, vmul/vaddy/vaddz, vsqrt, vdiv and vmulq on
   VU0 macro mode, then sqc2. C cannot emit these; intentional assembly. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/sceVu0Normalize/FUN_00125180.s", FUN_00125180);
