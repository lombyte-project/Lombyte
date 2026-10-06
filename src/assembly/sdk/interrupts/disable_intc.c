#include "asm.h"

/* Exact Sony EE kernel veneer: _DisableIntc. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/interrupts/disable_intc/_DisableIntc.s",
            _DisableIntc);
