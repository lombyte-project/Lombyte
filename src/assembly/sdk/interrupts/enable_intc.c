#include "asm.h"

/* Exact Sony EE kernel veneer: _EnableIntc. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/interrupts/enable_intc/_EnableIntc.s",
            _EnableIntc);
