#include "types.h"
#include "asm.h"

/* Sony libkernel: disable interrupts (di/sync.p loop on COP0 Status.EIE). */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/DIntr/DIntr.s", DIntr);
