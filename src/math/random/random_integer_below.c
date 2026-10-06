#include "types.h"
extern s32 FUN_001160d8();
s32 random_integer_below(s32 limit) __asm__("FUN_00213260");

s32 random_integer_below(s32 limit) {
    return (s32)((FUN_001160d8() >> 0x10) & 0x7FFF) % limit;
}

extern s32 func_00213260(s32 limit) __attribute__((alias("FUN_00213260")));
