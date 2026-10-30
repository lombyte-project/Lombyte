/* Runtime (asm): starts a DMA transfer through the channel at 0x1000D400, and three VU0 routines. */
#include "types.h"
#include "asm.h"

INCLUDE_ASM("config/us/overlays/asm/FUN_L02_002101b8.s", FUN_L02_002101b8);
INCLUDE_ASM("config/us/overlays/asm/FUN_L02_0024f748.s", FUN_L02_0024f748);
INCLUDE_ASM("config/us/overlays/asm/FUN_L02_0024fa80.s", FUN_L02_0024fa80);
INCLUDE_ASM("config/us/overlays/asm/FUN_L02_0024fca0.s", FUN_L02_0024fca0);
