#include "types.h"
#include "rnc/sdk/libmpeg.h"
#include "rnc/sdk/mpeg/errors.h"

struct MpegSkipState {
    s32 unk0;
    s32 unk4;
    u8  pad_8[0x8];
    s32 unk10;
    s32 unk14;
};

struct MpegMotionRef {
    s32 unk0;
    s32 unk4;
};

const char skipped_macroblock_in_i_picture_error_message[] __asm__("D_00153868") =
    "skiped macroblock in I picure is not allowed";
s32 _skipMB0(struct MpegDecoder   *decoder,
             struct MpegSkipState *skip_state,
             s32                  *picture_structure_mode,
             struct MpegMotionRef *motion_reference,

             s32 *flags) {
    enum {
        MPEG_PICTURE_CODING_TYPE_I = 1
    };
    enum {
        MPEG_MOTION_TYPE_FIELD = 1,
        MPEG_PICTURE_CODING_TYPE_P = 2,
        MPEG_MOTION_TYPE_FRAME = 2
    };
    s32 buf_offset;
    s32 bottom_field;
    s32 ok;

    ok = 1;
    buf_offset = decoder->mb_buf_index * 0x140;
    *(s32 *)((u8 *)(((u8 *)decoder + (buf_offset))) + 0x6CC) = 1;
    decoder->dc_reset = 1;
    if (decoder->picture_coding_type == MPEG_PICTURE_CODING_TYPE_P) {
        skip_state->unk14 = 0;
        skip_state->unk10 = 0;
        skip_state->unk4 = 0;
        skip_state->unk0 = 0;
    }
    if (decoder->picture_structure == 3) {
        *picture_structure_mode = MPEG_MOTION_TYPE_FRAME;
    } else {
        *picture_structure_mode = MPEG_MOTION_TYPE_FIELD;
        bottom_field = decoder->picture_structure == 2;
        motion_reference->unk4 = bottom_field;
        motion_reference->unk0 = bottom_field;
    }
    if (decoder->picture_coding_type == MPEG_PICTURE_CODING_TYPE_I) {
        _Error(decoder, skipped_macroblock_in_i_picture_error_message);
        ok = 0;
    }
    *flags &= ~1;
    return ok;
}
