#include "asm.h"

/* Exact Sony EE kernel veneer: SuspendThread. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/threads/suspend_thread/SuspendThread.s",
            SuspendThread);
