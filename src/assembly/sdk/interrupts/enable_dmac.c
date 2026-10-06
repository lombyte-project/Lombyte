#include "asm.h"

/* Exact Sony EE kernel veneer: _EnableDmac. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/interrupts/enable_dmac/_EnableDmac.s",
            _EnableDmac);
