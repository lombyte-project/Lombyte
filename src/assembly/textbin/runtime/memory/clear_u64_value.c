#include "types.h"
#include "asm.h"

/* Stores the zero register straight to memory (`sq $0`); the compiler's
   quadword move has no zero alternative and always goes through `por` first. */
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/runtime/memory/clear_u64_value/FUN_001f99f8.s", clear_u64_value);
