/*
 * sceDmaReset - EE DMA controller reset (Ratchet & Clank, SCUS_971.99).
 *
 * Exact (EE-GCC 2.9 profile, code/function/data 100, byte-equal via patha).
 *
 * Semantics:
 *   - capture and return the DMA interrupt/enable status bit (D_CTRL.STATUS)
 *   - clear the six channel registers (CHCR, TADR, MADR, ASR1, ASR0, SADR)
 *     of every in-use DMA channel
 *   - reset the DMA control register: write 0xFF1F, then keep only the
 *     upper-half masked bits (0xFF1F0000)
 *   - zero a 20-byte DMA environment block and install it via sceDmaPutEnv
 *   - if requested, set D_CTRL.STATUS again
 *
 * NOTE on the last DMA register write: the retail oracle keeps the masked
 * read in a temp and commits it with a plain store, which is what lets the
 * EE-GCC 2.9 scheduler place the store in the Memclr call delay slot.
 * Spelling it as a `volatile` read-modify-write compiles to the same
 * instructions but the store moves one slot earlier (100 -> 97.09).
 */

#include "types.h"

typedef struct sceDmaChan {
    s32 chcr;
    u8 pad_04[0xC];
    s32 madr;
    u8 pad_14[0x1C];
    s32 tadr;
    u8 pad_34[0xC];
    s32 asr0;
    u8 pad_44[0xC];
    s32 asr1;
    u8 pad_54[0x2C];
    s32 sadr;
} sceDmaChan;

extern sceDmaChan *DmaChannels[] __asm__("D_00132D70");
extern s32 DmaChannelsUsed[] __asm__("D_001533F8");
extern s32 Memclr();
extern s32 sceDmaPutEnv();

s32 sceDmaReset(s32 arg0) {
    s32 buf[5];
    s32 status;
    s32 i;
    u32 mask;

    status = *(volatile u32 *)0x1000E000 & 1;
    for (i = 9; i >= 0; i--) {
        if (DmaChannelsUsed[9 - i] != 0) {
            sceDmaChan *p = DmaChannels[9 - i];
            p->chcr = 0;
            p->tadr = 0;
            p->madr = 0;
            p->asr1 = 0;
            p->asr0 = 0;
            p->sadr = 0;
        }
    }
    *(volatile u32 *)0x1000E010 = 0xFF1F;
    mask = *(volatile u32 *)0x1000E010 & 0xFF1F0000;
    *(u32 *)0x1000E010 = mask;
    Memclr(buf, 0x14);
    sceDmaPutEnv(buf);
    if (arg0 == 1) {
        *(volatile u32 *)0x1000E000 |= 1;
    }
    return status;
}
