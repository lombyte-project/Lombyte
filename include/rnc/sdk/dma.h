#ifndef LOMBYTE_RNC_SDK_DMA_H
#define LOMBYTE_RNC_SDK_DMA_H

#include "rnc/sdk/sce_dma_chan.h"
#include "types.h"

struct DmaChannelRegs;

sceDmaChan *GetDmaChannel(u32) __asm__("sceDmaGetChan");
s32         sceDmaPause(volatile u32 *);
s32         sceDmaPutEnv();
s32         sceDmaReset(s32);
void        sceDmaSend(struct DmaChannelRegs *, s32);
void        SetD3Chcr(s32);
void        SetD4Chcr(s32);
void        SetD4ChcrVariant(s32);
void        WaitDma(volatile u32 *);

#endif /* LOMBYTE_RNC_SDK_DMA_H */
