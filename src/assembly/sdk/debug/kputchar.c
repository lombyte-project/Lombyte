#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/debug/kputchar/kputchar.s", kputchar);
#else
#include "types.h"

s32 kputchar(s32 ch) {
    /* Wait for the SIO transmit FIFO, then send only ch's low byte. */
    while (*(volatile u32 *)0x1000F130 & 0x8000) {
    }
    *(volatile u8 *)0x1000F180 = (u8)ch;
    return ch;
}

#endif /* NON_MATCHING */
