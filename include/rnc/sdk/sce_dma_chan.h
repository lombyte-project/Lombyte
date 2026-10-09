#ifndef LOMBYTE_RNC_SDK_SCE_DMA_CHAN_H
#define LOMBYTE_RNC_SDK_SCE_DMA_CHAN_H

#include "types.h"
#include "sda.h"

/* EE DMA channel register block. */
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

/* Register block of each of the ten DMA channels. */
extern sceDmaChan *DmaChannels[10] __asm__("D_00132D70") NOT_SDA;

#endif /* LOMBYTE_RNC_SDK_SCE_DMA_CHAN_H */
