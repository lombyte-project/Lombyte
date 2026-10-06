#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/runtime/dma/dma_to_spr/WriteDmaChannelRegisters.s",
            WriteDmaChannelRegisters);
#else
#include "types.h"

/* 0x1000D000 / 0x1000D400 are the D1 / D2 DMA channel register blocks
 * (0x400 stride): CHCR 0x00, MADR 0x10, QWC 0x20, SADR 0x80. */
struct DmaChanRegs {
    s32 chcr; /* 0x00 */
    s32 unk04[3];
    s32 madr; /* 0x10 */
    s32 unk14[3];
    s32 qwc; /* 0x20 */
    s32 unk24[0x17];
    s32 sadr; /* 0x80 */
};

void WriteDmaChannelRegisters(s32 address, s32 qwc, s32 destination) {
    volatile struct DmaChanRegs *const d = (volatile struct DmaChanRegs *)0x1000D400;
    d->sadr = destination;
    d->qwc = qwc;
    d->madr = address;
    d->chcr = 0x100;
}
#endif /* NON_MATCHING */
