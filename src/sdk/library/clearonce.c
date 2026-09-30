#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x590];
    s32 unk590;
    s32 unk594;
    u8 pad_598[0x138];
    s32 unk6D0;
    s32 unk6D4;
    u8 pad_6D8[0x138];
    s32 unk810;
};

extern void _ipuSetMPEG1();
void _clearOnce(struct M2c_arg0 *arg0) {
    u32 spr = 0x70000000;

    _ipuSetMPEG1(1);
    arg0->unk590 = spr;
    arg0->unk594 = spr + 0x1800;
    arg0->unk6D0 = spr + 0x1B00;
    arg0->unk6D4 = spr + 0x3300;
    arg0->unk810 = 0;
}
