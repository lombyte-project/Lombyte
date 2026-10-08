#include "types.h"
#include "rnc/sdk/libmpeg.h"
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
