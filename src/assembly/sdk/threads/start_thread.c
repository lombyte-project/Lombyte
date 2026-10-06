#include "asm.h"

/* Exact Sony EE kernel veneer: _StartThread. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/threads/start_thread/_StartThread.s",
            _StartThread);
