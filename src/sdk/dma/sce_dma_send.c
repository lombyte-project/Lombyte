#include "types.h"
#include "rnc/sdk/dma.h"
#include "rnc/sdk/system.h"

struct DmaChannelRegs {
    s32 chcr;
    u8  pad_4[0x1C];
    s32 qwc;
    u8  pad_24[0xC];
    s32 tadr;
};

void sceDmaSend(struct DmaChannelRegs *channel, s32 addr) {
    s32          phys_addr;
    register u32 sentinel;
    phys_addr = CheckAddress(addr);
    WaitDma(channel);
    sentinel = 0xFFFFFFFFu;
    if ((u32)channel->tadr != sentinel)
        channel->tadr = phys_addr;
    channel->qwc = 0;
    channel->chcr = (s32)((channel->chcr & ~0xC) | 0x105);
}
