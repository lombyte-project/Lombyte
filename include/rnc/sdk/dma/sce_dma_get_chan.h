#ifndef LOMBYTE_RNC_SDK_DMA_SCE_DMA_GET_CHAN_H
#define LOMBYTE_RNC_SDK_DMA_SCE_DMA_GET_CHAN_H

#include "rnc/sdk/sce_dma_chan.h"
#include "types.h"

sceDmaChan *GetDmaChannel(u32) __asm__("sceDmaGetChan");

#endif /* LOMBYTE_RNC_SDK_DMA_SCE_DMA_GET_CHAN_H */
