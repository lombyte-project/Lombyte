#include "types.h"
#include "rnc/sdk/libmpeg.h"
struct MpegSkipState {
    s32 unk0;
    s32 unk4;
    u8 pad_8[0x8];
    s32 unk10;
    s32 unk14;
};

struct MpegMotionRef {
    s32 unk0;
    s32 unk4;
};

extern u8 D_00153868[];
extern s32 _Error(struct MpegDecoder *a0, u8 *a1);
s32 _skipMB0(struct MpegDecoder *decoder, struct MpegSkipState *arg1, s32 *arg2, struct MpegMotionRef *arg3,
             s32 *flags) {
    s32 temp_10_11;
    s32 temp_2_32;
    s32 var_9_6;

    var_9_6 = 1;
    temp_10_11 = decoder->mb_buf_index * 0x140;
    *(s32 *)((u8 *)(((u8 *)decoder + (temp_10_11))) + 0x6CC) = 1;
    decoder->dc_reset = 1;
    if (decoder->picture_coding_type == 2) {
        arg1->unk14 = 0;
        arg1->unk10 = 0;
        arg1->unk4 = 0;
        arg1->unk0 = 0;
    }
    if (decoder->picture_structure == 3) {
        *arg2 = 2;
    } else {
        *arg2 = 1;
        temp_2_32 = decoder->picture_structure == 2;
        arg3->unk4 = temp_2_32;
        arg3->unk0 = temp_2_32;
    }
    if (decoder->picture_coding_type == 1) {
        _Error(decoder, D_00153868);
        var_9_6 = 0;
    }
    *flags &= ~1;
    return var_9_6;
}
