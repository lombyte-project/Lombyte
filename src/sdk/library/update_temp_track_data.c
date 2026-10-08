#include "types.h"
#include "rnc/sdk/libmpeg.h"
void UpdateTempTrackData(struct MpegDecoder *arg0, s32 delta) {
    s32 temp_2_30;
    s32 temp_3_21;
    s32 temp_4_32;
    s32 var_4_8;
    s32 var_7_4;
    var_7_4 = 0;
    var_4_8 = 0;
    if ((arg0->picture_coding_type != 3) && (delta != 0)) {
        if (delta < 0) {
            var_7_4 = arg0->unk854 == 0;
        }
        arg0->unk854 = 0;
        var_4_8 = delta;
    }
    temp_3_21 = arg0->unk84C + delta;
    arg0->unk1AC = temp_3_21;
    if ((var_7_4 != 0) && (var_4_8 >= delta)) {
        arg0->unk1AC = (s32)(temp_3_21 + 0x400);
    };
    temp_4_32 = arg0->unk1AC;
    arg0->unk850 = (s32)((arg0->unk850 < temp_4_32) ? (temp_4_32) : (arg0->unk850));
}
