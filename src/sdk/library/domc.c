#include "types.h"
#include "rnc/sdk/libmpeg.h"

extern u8 D_00153770[];
extern s32 _Error();
extern void func_001271D8();
extern void func_00127178();

void _doMC(struct MpegDecoder *d, s32 index) {
    s32 i;

    if (d->mb_buf[index].unk138 != 0) {
        for (i = 0; i < d->mb_buf[index].fetch_count; i++) {
            ((void (*)(struct MpegMcFetch *))d->mb_buf[index].luma_func[i])(&d->mb_buf[index].luma[i]);
            ((void (*)(struct MpegMcFetch *))d->mb_buf[index].chroma_func[i])(&d->mb_buf[index].chroma[i]);
        }
    }
    if (d->mb_buf[index].unk130 != 0 && d->mb_buf[index].unk13C != 0) {
        _Error(d, D_00153770);
    }
    if (d->mb_buf[index].unk130 != 0) {
        func_001271D8(d->mb_buf[index].unk128, d->mb_buf[index].ipu_out);
    } else if (d->mb_buf[index].unk13C != 0) {
        func_001271D8(d->mb_buf[index].unk128, d->mc_work);
    } else {
        func_00127178(d->mb_buf[index].unk128, d->mc_work, d->mb_buf[index].ipu_out);
    }
}
