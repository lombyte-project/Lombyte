#include "asm.h"

/* Exact Sony EE kernel veneer: FlushCache. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/memory/cache/flush_cache/FlushCache.s",
            FlushCache);
