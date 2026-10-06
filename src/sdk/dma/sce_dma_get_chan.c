#include "types.h"

typedef struct DmaChannel DmaChannel;

extern DmaChannel *DmaChannels[10] __asm__("D_00132D70") __attribute__((section(".data")));

DmaChannel *GetDmaChannel(u32 channel_number) __asm__("sceDmaGetChan");

DmaChannel *GetDmaChannel(u32 channel_number) {
    if (channel_number < 10u) {
        return DmaChannels[channel_number];
    }
    return NULL;
}
