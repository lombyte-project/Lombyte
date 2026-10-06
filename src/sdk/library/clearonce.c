#include "types.h"
struct MpegDecoder {
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
void _clearOnce(struct MpegDecoder *mpeg) {
    u32 spr = 0x70000000;

    _ipuSetMPEG1(1);
    mpeg->unk590 = spr;
    mpeg->unk594 = spr + 0x1800;
    mpeg->unk6D0 = spr + 0x1B00;
    mpeg->unk6D4 = spr + 0x3300;
    mpeg->unk810 = 0;
}
