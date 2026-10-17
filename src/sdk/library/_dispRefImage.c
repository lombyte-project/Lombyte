#include "types.h"
#include "rnc/sdk/libmpeg.h"
/* s32 view of the owning sceMpeg (pts low word at 0x10, flags at 0x20). */
struct MpegDisplayState {
    u8 pad_0[0x10];
    s32 unk10;
    u8 pad_14[0xC];
    s64 unk20;
};

extern u32 D_00132E98[];
extern s32 _cpr8();
extern s32 _csc_storeRefImage();
extern s32 _getPtsDtsFlags();
extern s32 _isOutSizeOK();
void _dispRefImage(struct MpegDecoder *decoder, struct MpegRefImage *ref_image) {
    u64 temp_6_28;
    struct MpegDisplayState *ref_info;
    s32 *temp_7_9;

    temp_7_9 = (s32 *)decoder->mpeg;
    _getPtsDtsFlags(decoder, ref_image, (s32 *)((u8 *)temp_7_9 + 0x10), (s32 *)((u8 *)temp_7_9 + 0x18),
                    (s32 *)((u8 *)temp_7_9 + 0x20));
    ref_info = (struct MpegDisplayState *)decoder->mpeg;
    decoder->unk80 = (s32)ref_info->unk10;
    temp_6_28 = D_00132E98[(s32)(ref_info->unk20 >> 5) & 0xF];
    decoder->unkCC = (s32)ref_image->unk5C;
    decoder->unk88 = temp_6_28;
    decoder->unkD0 = (s32)ref_image->unk60;
    decoder->unkB4 = (s32)ref_image->unk44;
    decoder->unkB8 = (s32)ref_image->unk48;
    decoder->unkBC = (s32)ref_image->unk4C;
    decoder->unkC0 = (s32)ref_image->unk50;
    decoder->unkC4 = (s32)ref_image->unk54;
    decoder->unkC8 = (s32)ref_image->unk58;
    if (_isOutSizeOK(decoder, ref_image) != 0) {
        if (ref_image->status == 1) {
            if (decoder->unkB0 != 0) {
                _csc_storeRefImage(decoder, ref_image);
            } else {
                _cpr8(decoder, ref_image);
            }
            func_00129B38(decoder);
        }
    }
}
