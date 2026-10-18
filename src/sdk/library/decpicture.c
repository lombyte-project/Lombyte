#include "types.h"

struct MpegReferenceImage;

struct MpegDecoderState {
    u8 reserved_000[0x120];
    s32 second_field;
    u8 reserved_124[0x50];
    s32 picture_coding_type;
    u8 reserved_178[0x48];
    struct MpegReferenceImage *forward_reference;
    u8 reserved_1C4[0xC];
    struct MpegReferenceImage *backward_reference;
    u8 reserved_1D4[0xC];
    struct MpegReferenceImage *bidirectional_reference;
};

struct MpegReferenceImage {
    u8 reserved_00[0x28];
    s32 decoded;
};

extern u8 D_00153940[];
extern u8 D_00153960[];
extern s32 _Error();
extern s32 _pictureData0();

s32 _decPicture(struct MpegDecoderState *decoder) {
    enum {
        FRAME_PICTURE = 3
    };
    s32 result;
    struct MpegReferenceImage *reference;

    if (decoder->picture_coding_type == 3 && decoder->second_field != 0) {
        _Error(decoder, D_00153940);
        decoder->second_field = 0;
    }
    switch (decoder->picture_coding_type) {
    case FRAME_PICTURE:
        reference = decoder->forward_reference;
        break;
    case 1:
        reference = decoder->backward_reference;
        break;
    case 2:
        reference = decoder->bidirectional_reference;
        break;
    default:
        reference = decoder->forward_reference;
        _Error(decoder, D_00153960);
        break;
    }
    result = _pictureData0(decoder);
    if (result != 0) {
        reference->decoded = 1;
    }
    return result;
}
