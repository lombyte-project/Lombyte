#include "types.h"
struct DmaChannelRegs {
    s32 unk0;
    u8 pad_4[0x1C];
    s32 unk20;
    u8 pad_24[0xC];
    s32 unk30;
};
extern s32 CheckAddress();
extern void WaitDma();
void sceDmaSend(struct DmaChannelRegs *channel, s32 addr) {
    s32 phys_addr;
    register u32 sentinel;
    phys_addr = CheckAddress(addr);
    WaitDma(channel);
    sentinel = 0xFFFFFFFFu;
    if ((u32)channel->unk30 != sentinel)
        channel->unk30 = phys_addr;
    channel->unk20 = 0;
    channel->unk0 = (s32)((channel->unk0 & ~0xC) | 0x105);
}
