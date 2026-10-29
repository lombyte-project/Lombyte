#include "types.h"
#include "rnc/sdk/libmpeg.h"

extern s32 _decPicture();
extern s32 _dispatchMpegCbNodata();
extern s32 _nextHeader();
extern s32 _sceMpegFlush();
extern s32 _updateRefImage();
extern void _outputFrame();
s32 _decodeOrSkipField(struct sceMpeg *decoder, s32 skip_comparison_value, s32 skip_threshold) {
    struct MpegDecoder *mpeg_decoder;
    s32 should_decode;
    unsigned int new_var2;
    s32 expected_second_field_structure;
    long helper_result;
    s32 frame_count_snapshot;
    s32 second_field_decode_nonzero;
    short new_var;
    s32 gate;
    should_decode = 0;
    mpeg_decoder = decoder->sys;
    mpeg_decoder->second_field = 0;
    if ((skip_threshold == (-1)) || (skip_comparison_value < skip_threshold)) {
        should_decode = 1;
    };
    if (mpeg_decoder->unk8 == 0) {
        decoder->frameCount = 0;
        mpeg_decoder->unk8 = 1;
    }
    helper_result = _updateRefImage(mpeg_decoder, 0);
    if ((helper_result != 0) && (should_decode != 0)) {
        _decPicture(mpeg_decoder);
    }
    mpeg_decoder->second_field = 1;
    helper_result = _nextHeader(mpeg_decoder);
    if (helper_result == 0) {
        _sceMpegFlush(decoder);
        mpeg_decoder->unk0 = 1;
        return 0;
    }
    expected_second_field_structure = 2;
    if (mpeg_decoder->unkD4 != 1) {
        expected_second_field_structure = 1;
    }
    if (mpeg_decoder->picture_structure != expected_second_field_structure) {
        return -1;
    }
    new_var2 = _updateRefImage(mpeg_decoder, 1);
    gate = 0;
    if (new_var2 != 0) {
        gate = 1;
    }
    second_field_decode_nonzero = 0;
    if (gate != 0) {
        if (should_decode == 0) {
            frame_count_snapshot = mpeg_decoder->frame_count;
            goto out;
        }
        helper_result = _decPicture(mpeg_decoder);
        if (helper_result != 0) {
            second_field_decode_nonzero = 1;
        }
    };
out:
    _outputFrame(mpeg_decoder, mpeg_decoder->frame_count, mpeg_decoder->unk4);

    mpeg_decoder->second_field = 0;
    decoder->frameCount = mpeg_decoder->frame_count - mpeg_decoder->frame_base;
    mpeg_decoder->frame_count = mpeg_decoder->frame_count + 1;
    mpeg_decoder->unk4 = (unsigned long long)(mpeg_decoder->unk4 + 1);
    if (should_decode == 0) {
        _dispatchMpegCbNodata(decoder);
    }
    return second_field_decode_nonzero;
}
