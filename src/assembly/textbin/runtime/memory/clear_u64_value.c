#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/runtime/memory/clear_u64_value/FUN_001f99f8.s",
            FUN_001f99f8);
#else
#include "types.h"
#include "eetypes.h"

void clear_u64_value(s64 *dst) __asm__("FUN_001f99f8");

void clear_u64_value(s64 *dst) {
    /* Retail stores a full 16-byte quadword through dst. */
    *(u128 *)dst = 0;
}

extern void func_001F99F8(s64 *dst) __attribute__((alias("FUN_001f99f8")));
#endif /* NON_MATCHING */
