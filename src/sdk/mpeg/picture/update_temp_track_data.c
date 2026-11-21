#include "types.h"
#include "rnc/sdk/libmpeg.h"

void UpdateTempTrackData(struct MpegDecoder *decoder, s32 delta) {
    enum {
        MPEG_PICTURE_CODING_TYPE_B = 3
    };
    s32 reference;
    s32 current;
    s32 step;
    s32 wrapped;
    wrapped = 0;
    step = 0;
    if ((decoder->picture_coding_type != MPEG_PICTURE_CODING_TYPE_B) && (delta != 0)) {
        if (delta < 0) {
            wrapped = decoder->unk854 == 0;
        }
        decoder->unk854 = 0;
        step = delta;
    }
    reference = decoder->temporal_reference_base + delta;
    decoder->temporal_reference = reference;
    if ((wrapped != 0) && (step >= delta)) {
        decoder->temporal_reference = (s32)(reference + 0x400);
    };
    current = decoder->temporal_reference;
    decoder->max_temporal_reference =
        (s32)((decoder->max_temporal_reference < current) ? (current)
                                                          : (decoder->max_temporal_reference));
}
