#include "asm.h"

#include "types.h"
struct MpegDecoder {
    u8 pad_0[0xF8];
    s32 unkF8;
    u8 pad_FC[0x54];
    s32 unk150;
    u8 pad_154[0x20];
    s32 unk174;
    u8 pad_178[0x40];
    s32 unk1B8;
    u8 pad_1BC[0x8];
    s32 unk1C4;
    s32 unk1C8;
    u8 pad_1CC[0x8];
    s32 unk1D4;
    s32 unk1D8;
    u8 pad_1DC[0x8];
    s32 unk1E4;
};

extern s32 _dispRefImage(struct MpegDecoder *mpeg, s32 arg1, s32 arg2);
extern s32 _dispRefImageField(struct MpegDecoder *mpeg, s32 arg1, s32 arg2, s32 arg3);
void _outputFrame(struct MpegDecoder *mpeg, s32 arg1, s32 arg2) {
    s32 var_5_14;
    s32 var_5_24;
    s32 var_6_27;

    if (arg2 != 0) {
        if (mpeg->unk174 == 3) {
            if (mpeg->unk150 == 3) {
                var_5_14 = mpeg->unk1C4;
            } else {
                var_5_14 = mpeg->unk1B8;
            }
            _dispRefImage(mpeg, var_5_14, arg1 - 1);
        } else {
            if (mpeg->unk150 == 3) {
                var_5_24 = mpeg->unk1D4;
                var_6_27 = mpeg->unk1E4;
            } else {
                var_5_24 = mpeg->unk1C8;
                var_6_27 = mpeg->unk1D8;
            }
            _dispRefImageField(mpeg, var_5_24, var_6_27, arg1 - 1);
        }
    }
    if (mpeg->unkF8 == 1) {
        mpeg->unkF8 = 2;
    }
}
