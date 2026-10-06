#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact low-cost entry recovered with target symbolic relocations. */
INCLUDE_ASM("config/us/expected/asm/assembly/runtime/dma/write_dma_channel/WriteDmaChannel.s",
            WriteDmaChannel);
#else
#include "types.h"

/* 0x1000D000 / 0x1000D400 are the D1 / D2 DMA channel register blocks
 * (0x400 stride): CHCR 0x00, SADR 0x10, DAR 0x20, CHCNT 0x80. */
struct DmaChanRegs {
    s32 chcr; /* 0x00 */
    s32 unk04[3];
    s32 sadr; /* 0x10 */
    s32 unk14[3];
    s32 dar; /* 0x20 */
    s32 unk24[0x17];
    s32 chcnt; /* 0x80 */
};

s32 WriteDmaChannel(s32 sadr, s32 dar, s32 chcnt) {
    volatile struct DmaChanRegs *d = (volatile struct DmaChanRegs *)0x1000D000;
    d->chcnt = dar;
    d->sadr = sadr;
    d->dar = chcnt;
    d->chcr = 0x100;
    if (*(volatile u32 *)0x20100000 & 1u) {
        return 0x100;
    }
    return 0x100;
}
#endif /* NON_MATCHING */
