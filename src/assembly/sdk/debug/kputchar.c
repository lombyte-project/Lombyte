#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/debug/kputchar/kputchar.s", kputchar);
#else
#include "types.h"

s32 kputchar(s32 arg0) {
    volatile u32 *status = (volatile u32 *)0x1000F130;
    volatile u8 *output = (volatile u8 *)0x1000F180;

    while (*status & 0x8000) {
    }
    *output = (u8)arg0;
    return arg0;
}

#endif /* NON_MATCHING */
