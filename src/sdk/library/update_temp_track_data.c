#include "types.h"
#include "rnc/sdk/libmpeg.h"
void UpdateTempTrackData(struct MpegDecoder *decoder, s32 delta) {
    s32 temp_2_30;
    s32 temp_3_21;
    s32 temp_4_32;
    s32 var_4_8;
    s32 var_7_4;
    var_7_4 = 0;
    var_4_8 = 0;
    if ((decoder->picture_coding_type != 3) && (delta != 0)) {
        if (delta < 0) {
            var_7_4 = decoder->unk854 == 0;
        }
        decoder->unk854 = 0;
        var_4_8 = delta;
    }
    temp_3_21 = decoder->unk84C + delta;
    decoder->unk1AC = temp_3_21;
    if ((var_7_4 != 0) && (var_4_8 >= delta)) {
        decoder->unk1AC = (s32)(temp_3_21 + 0x400);
    };
    temp_4_32 = decoder->unk1AC;
    decoder->unk850 = (s32)((decoder->unk850 < temp_4_32) ? (temp_4_32) : (decoder->unk850));
}
