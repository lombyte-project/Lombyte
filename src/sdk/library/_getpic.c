#include "types.h"
#include "rnc/sdk/library/_getpic.h"

extern u8 D_00153A58[];
extern s32 _Error1();
extern s32 _decodeOrSkip();
extern s32 _nextHeader();
extern s32 _sceMpegFlush();

s32 _getpic(struct MpegDecoder *mpeg) {
    s32 temp_6_13;
    s32 var_18_8;
    u32 var_19_6;
    struct MpegPictureContext *temp_16_12;

    var_19_6 = 1;
    var_18_8 = 0;
    temp_16_12 = mpeg->unk40;
    temp_6_13 = temp_16_12->unkD8;
    temp_16_12->unk0 = 0;
    if (temp_6_13 & 0x3F) {
        _Error1(temp_16_12, D_00153A58, temp_6_13);
        return -1;
    }
    temp_16_12->unk820 = 0;
    do {
        if (var_18_8 == -1) {
            goto dispatch;
        }
        do {
            var_19_6 = _nextHeader(temp_16_12);
            if (var_19_6 == 0) {
                break;
            }
            if (temp_16_12->unk174 == temp_16_12->unkD4) {
                break;
            }
        } while (temp_16_12->unk848 != 0);
    dispatch:
        switch (var_19_6) {
        case 0:
            _sceMpegFlush(mpeg);
            temp_16_12->unk0 = 1;
            break;
        case 1:
            temp_16_12->unkA8 = 0;
            temp_16_12->unkA4 = 0;
            temp_16_12->unkA0 = 0;
            var_18_8 = _decodeOrSkip(mpeg, 0, temp_16_12->unk94);
            temp_16_12->unkA0 = (s32)(temp_16_12->unkA0 + 1);
            break;
        case 2:
            var_18_8 = _decodeOrSkip(mpeg, temp_16_12->unkA4, temp_16_12->unk98);
            temp_16_12->unkA4 = (s32)(temp_16_12->unkA4 + 1);
            break;
        case 3:
        case 4:
            var_18_8 = _decodeOrSkip(mpeg, temp_16_12->unkA8, temp_16_12->unk9C);
            temp_16_12->unkA8 = (s32)(temp_16_12->unkA8 + 1);
            break;
        }
        if (temp_16_12->unk820 != 0) {
            return 1;
        }
    } while (temp_16_12->unk0 == 0);
    return 1;
}
