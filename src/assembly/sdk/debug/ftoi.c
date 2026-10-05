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
    t = v << 0xC;
    input = t >> 0xC;
    v = input | ((u64)0x8000 << 0x25);
    if (exp < 0) {
        exp = -exp;
        v = v >> (exp - 2);
        if ((v & 3) == 3) {
            v = (v >> 2) + 1;
        } else {
            v = (v >> 2);
        }
    } else {
        v = v << exp;
    }
    return (s64)(v << 0x20) >> 0x20;
}

#endif /* NON_MATCHING */
