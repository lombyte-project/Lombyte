#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/math/sign_extend_packed_value/SignExtendPackedValue.s", SignExtendPackedValue);
#else
#include "types.h"

s64 SignExtendPackedValue(u64 *arg0, s32 arg1) {
    u64 w;

    w = *arg0 >> (0x40 - arg1);
    w = w << 0x20;
    __asm__ __volatile__("" : "+r"(w));
    return (s64) w >> 0x20;
}

#endif /* NON_MATCHING */
