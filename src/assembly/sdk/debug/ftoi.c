#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/debug/ftoi/ftoi.s", ftoi);
#else
#include "types.h"

s64 ftoi(s64 arg) {
    s64 input;
    u64 v;
    s64 exp;
    u64 t;
    input = arg;
    v = (u64)input;
    exp = (s64)((v << 1) >> 0x35);
    exp -= 0x433;
    if (exp < -0x35) {
        return 0;
    }
    if (exp >= 0xD) {
        return 0x270F;
    }
    t = (u64)input << 0xC;
    input = (u64)t >> 0xC;
    input = (u64)input | ((u64)0x8000 << 0x25);
    if (exp < 0) {
        exp = -exp;
        /* The retail dsrlv masks this shift count, including exp == 1. */
        input = (u64)input >> (exp - 2);
        if ((input & 3) == 3) {
            input = ((u64)input >> 2) + 1;
        } else {
            input = (u64)input >> 2;
        }
    } else {
        input = (u64)input << exp;
    }
    input = (s32)input;
    return input;
}

#endif /* NON_MATCHING */
