#include "types.h"
#include "rnc/sdk/libmpeg.h"
#include "rnc/sdk/mpeg/api.h"
#include "rnc/sdk/mpeg/picture.h"
#include "rnc/sdk/mpeg/ref_images.h"

s32 _decodeOrSkipFrame(struct sceMpeg *decoder, s32 skip_comparison_value, s32 skip_threshold) {
    s32                 decode_or_skip_result;
    s32                 flag;
    s32                 decode_succeeded;
    struct MpegDecoder *mpeg_decoder;
    flag = 0;
    mpeg_decoder = decoder->sys;
    if ((skip_threshold == -1) || (skip_comparison_value < skip_threshold)) {
        if (mpeg_decoder->started == 0) {
            decoder->frameCount = 0;
            mpeg_decoder->started = 1;
        }
        if (UpdateMpegReferenceImages(mpeg_decoder, 0) == 0) {
            decode_succeeded = 0;
        } else {
            decode_succeeded = 0;
            decode_succeeded = _decPicture(mpeg_decoder) != decode_succeeded;
        }
        decode_or_skip_result = decode_succeeded;
    } else {
        flag = 1;
        decode_or_skip_result = UpdateMpegReferenceImages(mpeg_decoder, 0);
        DispatchMpegCallbackNoData(decoder);
    }
    _outputFrame(mpeg_decoder, mpeg_decoder->frame_count, mpeg_decoder->decoded_pictures);
    if ((mpeg_decoder->picture_structure != 3) && (flag == 0)) {
        mpeg_decoder->second_field = (s32)(mpeg_decoder->second_field == 0);
    }
    decoder->frameCount = (s32)(mpeg_decoder->frame_count - mpeg_decoder->frame_base);
    if (mpeg_decoder->second_field == 0) {
        mpeg_decoder->frame_count = (s32)(mpeg_decoder->frame_count + 1);
        mpeg_decoder->decoded_pictures = (s32)(mpeg_decoder->decoded_pictures + 1);
    }
    return decode_or_skip_result;
}
