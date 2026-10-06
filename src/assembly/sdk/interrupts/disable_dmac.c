#include "asm.h"

/* Exact Sony EE kernel veneer: _DisableDmac. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/interrupts/disable_dmac/_DisableDmac.s",
            _DisableDmac);
