#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/debug/kputchar/kputchar.s", kputchar);
#else
#include "types.h"

s32 kputchar(s32 arg0) {
    volatile u8 * const tx = (volatile u8 *)0x1000F180;
    /* Wait for the SIO transmit FIFO, then send only arg0's low byte. */
    while (*(volatile u32 *)0x1000F130 & 0x8000) {
    }
    *tx = (u8)arg0;
    return arg0;
}

#endif /* NON_MATCHING */
