#include "types.h"
#include "rnc/sdk/libmpeg.h"

extern u8   intra_skip_mb_error_message[] __asm__("D_00153770");
extern s32  _Error();
extern void func_001271D8();
extern void func_00127178();

void _doMC(struct MpegDecoder *decoder, s32 index) {
    s32 i;

    if (decoder->mb_buf[index].unk138 != 0) {
        for (i = 0; i < decoder->mb_buf[index].fetch_count; i++) {
            ((void (*)(struct MpegMcFetch *))decoder->mb_buf[index].luma_func[i])(
                &decoder->mb_buf[index].luma[i]);
            ((void (*)(struct MpegMcFetch *))decoder->mb_buf[index].chroma_func[i])(
                &decoder->mb_buf[index].chroma[i]);
        }
    }
    if (decoder->mb_buf[index].unk130 != 0 && decoder->mb_buf[index].unk13C != 0) {
        _Error(decoder, intra_skip_mb_error_message);
    }
    if (decoder->mb_buf[index].unk130 != 0) {
        func_001271D8(decoder->mb_buf[index].unk128, decoder->mb_buf[index].ipu_out);
    } else if (decoder->mb_buf[index].unk13C != 0) {
        func_001271D8(decoder->mb_buf[index].unk128, decoder->mc_work);
    } else {
        func_00127178(
            decoder->mb_buf[index].unk128, decoder->mc_work, decoder->mb_buf[index].ipu_out);
    }
}
