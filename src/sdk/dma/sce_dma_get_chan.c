#include "types.h"

typedef struct sceDmaChan sceDmaChan;

extern sceDmaChan *DmaChannels[10] __asm__("D_00132D70") __attribute__((section(".data")));

sceDmaChan *GetDmaChannel(u32 channel_number) __asm__("sceDmaGetChan");

sceDmaChan *GetDmaChannel(u32 channel_number) {
    if (channel_number < 10u) {
        return DmaChannels[channel_number];
    }
    return NULL;
}
