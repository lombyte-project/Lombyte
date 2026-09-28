#include "types.h"

extern u32 D_0015FD64 __attribute__((sda));

s32 FUN_00206b78(void) __asm__("FUN_00206b78");

s32 FUN_00206b78(void) {
    return D_0015FD64 < 1;
}

extern __typeof__(FUN_00206b78) func_00206B78 __attribute__((alias("FUN_00206b78")));
