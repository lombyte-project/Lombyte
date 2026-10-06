#include "types.h"
#include "asm.h"

#include "types.h"

extern u32 *D_00159A28 __attribute__((section(".bss")));
extern u32 *D_00159A2C __attribute__((section(".bss")));
extern u32 *D_00159A30 __attribute__((section(".bss")));

void FUN_00123d10(u32 address) {
    u32 *source = (u32 *)(address | 0x20000000);

    if (D_00159A28 != 0) {
        *D_00159A28 = source[0];
    }
    if (D_00159A2C != 0) {
        *D_00159A2C = source[1];
    }
    if (D_00159A30 != 0) {
        *D_00159A30 = source[0x24];
    }
}

void D_00123D10(u32 address) __attribute__((alias("FUN_00123d10")));
