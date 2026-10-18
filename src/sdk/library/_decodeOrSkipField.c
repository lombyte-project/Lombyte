#include "types.h"
#include "rnc/sdk/libmpeg.h"

extern s32 _decPicture();
extern s32 _dispatchMpegCbNodata();
extern s32 _nextHeader();
extern s32 _sceMpegFlush();
extern s32 _updateRefImage();
extern void _outputFrame();
s32 _decodeOrSkipField(struct sceMpeg *decoder, s32 skip_comparison_value, s32 skip_threshold) {
    struct MpegDecoder *p;
    s32 should_decode;
    unsigned int new_var2;
    s32 expected_second_field_structure;
    long helper_result;
    s32 frame_count_snapshot;
    s32 second_field_decode_nonzero;
    short new_var;
    s32 gate;
    should_decode = 0;
    p = decoder->sys;
    p->second_field = 0;
    if ((skip_threshold == (-1)) || (skip_comparison_value < skip_threshold)) {
        should_decode = 1;
    };
    if (p->unk8 == 0) {
        decoder->frameCount = 0;
        p->unk8 = 1;
    }
    helper_result = _updateRefImage(p, 0);
    if ((helper_result != 0) && (should_decode != 0)) {
        _decPicture(p);
    }
    p->second_field = 1;
    helper_result = _nextHeader(p);
    if (helper_result == 0) {
        _sceMpegFlush(decoder);
        p->unk0 = 1;
        return 0;
    }
    expected_second_field_structure = 2;
    if (p->unkD4 != 1) {
        expected_second_field_structure = 1;
    }
    if (p->picture_structure != expected_second_field_structure) {
        return -1;
    }
    new_var2 = _updateRefImage(p, 1);
    gate = 0;
    if (new_var2 != 0) {
        gate = 1;
    }
    second_field_decode_nonzero = 0;
    if (gate != 0) {
        if (should_decode == 0) {
            frame_count_snapshot = p->frame_count;
            goto out;
        }
        helper_result = _decPicture(p);
        if (helper_result != 0) {
            second_field_decode_nonzero = 1;
        }
    };
out:
    _outputFrame(p, p->frame_count, p->unk4);

    p->second_field = 0;
    decoder->frameCount = p->frame_count - p->frame_base;
    p->frame_count = p->frame_count + 1;
    p->unk4 = (unsigned long long)(p->unk4 + 1);
    if (should_decode == 0) {
        _dispatchMpegCbNodata(decoder);
    }
    return second_field_decode_nonzero;
}
