#include "types.h"

s32 FUN_0011a448(s32 *a0, s32 *a1) __asm__("FUN_0011a448");

s32 FUN_0011a448(s32 *a0, s32 *a1) {
    s32 value;

    value = *(s32 *)((u8 *)a0 + 0x10);
    *(s32 *)((u8 *)a1 + 0x8) = value;
    return value;
}

extern __typeof__(FUN_0011a448) func_0011A448 __attribute__((alias("FUN_0011a448")));
