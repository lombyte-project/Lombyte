#include "types.h"
struct M2c_arg0 { s32 unk0; u8 pad_4[0x1C]; s32 unk20; u8 pad_24[0xC]; s32 unk30; };
extern s32 CheckAddress();
extern void WaitDma();
void sceDmaSend(struct M2c_arg0 *arg0, s32 arg1) {
    s32 temp_17_10;
    register u32 sentinel;
    temp_17_10 = CheckAddress(arg1);
    WaitDma(arg0);
    sentinel = 0xFFFFFFFFu;
    if ((u32)arg0->unk30 != sentinel) arg0->unk30 = temp_17_10;
    arg0->unk20 = 0;
    arg0->unk0 = (s32)((arg0->unk0 & ~0xC) | 0x105);
}
