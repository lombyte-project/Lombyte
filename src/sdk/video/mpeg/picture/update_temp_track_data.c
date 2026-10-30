#include "types.h"
#include "rnc/sdk/libmpeg.h"

void UpdateTempTrackData(struct MpegDecoder *decoder, s32 delta) {
    s32 reference;
    s32 current;
    s32 step;
    s32 wrapped;
    wrapped = 0;
    step = 0;
    if ((decoder->picture_coding_type != 3) && (delta != 0)) {
        if (delta < 0) {
            wrapped = decoder->unk854 == 0;
        }
        decoder->unk854 = 0;
        step = delta;
    }
    reference = decoder->unk84C + delta;
    decoder->unk1AC = reference;
    if ((wrapped != 0) && (step >= delta)) {
        decoder->unk1AC = (s32)(reference + 0x400);
    };
    current = decoder->unk1AC;
    decoder->unk850 = (s32)((decoder->unk850 < current) ? (current) : (decoder->unk850));
}
