#include "types.h"
#include "rnc/sdk/sce_dma_chan.h"

sceDmaChan *GetDmaChannel(u32 channel_number) __asm__("sceDmaGetChan");

sceDmaChan *GetDmaChannel(u32 channel_number) {
    if (channel_number < 10u) {
        return DmaChannels[channel_number];
    }
    return NULL;
}
