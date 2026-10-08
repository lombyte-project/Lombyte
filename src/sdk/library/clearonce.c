#include "types.h"
#include "rnc/sdk/libmpeg.h"
extern void _ipuSetMPEG1();
void _clearOnce(struct MpegDecoder *mpeg) {
    u32 spr = 0x70000000;

    _ipuSetMPEG1(1);
    mpeg->mb_buf[0].spr_base = spr;
    mpeg->mb_buf[0].ipu_out = spr + 0x1800;
    mpeg->mb_buf[1].spr_base = spr + 0x1B00;
    mpeg->mb_buf[1].ipu_out = spr + 0x3300;
    mpeg->mb_buf_index = 0;
}
