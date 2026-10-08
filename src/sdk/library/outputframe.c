#include "asm.h"

#include "types.h"
#include "rnc/sdk/libmpeg.h"
extern s32 _dispRefImage(struct MpegDecoder *mpeg, s32 arg1, s32 arg2);
extern s32 _dispRefImageField(struct MpegDecoder *mpeg, s32 arg1, s32 arg2, s32 arg3);
void _outputFrame(struct MpegDecoder *mpeg, s32 arg1, s32 arg2) {
    s32 var_5_14;
    s32 var_5_24;
    s32 var_6_27;

    if (arg2 != 0) {
        if (mpeg->picture_structure == 3) {
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
