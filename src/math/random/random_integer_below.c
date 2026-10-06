#include "types.h"
extern s32 FUN_001160d8();
s32 random_integer_below(s32 arg0) __asm__("FUN_00213260");

s32 random_integer_below(s32 arg0) {
    return (s32)((FUN_001160d8() >> 0x10) & 0x7FFF) % arg0;
}

extern s32 func_00213260(s32 arg0) __attribute__((alias("FUN_00213260")));
