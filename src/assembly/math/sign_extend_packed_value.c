#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/math/sign_extend_packed_value/SignExtendPackedValue.s",
            SignExtendPackedValue);
#else
#include "types.h"

s32 SignExtendPackedValue(u64 *arg0, s32 arg1) {
    u64 shifted = *arg0 >> (0x40 - arg1);
    s32 result = shifted;
    return result;
}

#endif /* NON_MATCHING */
