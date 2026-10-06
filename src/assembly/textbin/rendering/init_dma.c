#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/init_dma/FUN_0020b418.s",
            FUN_0020b418);
#else
#include "types.h"

void init_dma(void) __asm__("FUN_0020b418");

void init_dma(void) {
    u32 priority;
    struct DmaChannel { u32 unused[8]; vu32 qwc; };
    priority = *(vu32 *)0x1000e020;
    *(vu32 *)0x1000e020 = priority | 0x200;
    *(vu32 *)0x1000e000 = 1;
    ((volatile struct DmaChannel *)0x10008000)->qwc = 0; /* VIF0 */
    ((volatile struct DmaChannel *)0x10009000)->qwc = 0; /* VIF1 */
    ((volatile struct DmaChannel *)0x1000a000)->qwc = 0; /* GIF */
    ((volatile struct DmaChannel *)0x1000b000)->qwc = 0; /* IPU from */
    ((volatile struct DmaChannel *)0x1000b400)->qwc = 0; /* IPU to */
    ((volatile struct DmaChannel *)0x1000d000)->qwc = 0; /* SPR from */
    ((volatile struct DmaChannel *)0x1000d400)->qwc = 0; /* SPR to */
}
#endif /* NON_MATCHING */
