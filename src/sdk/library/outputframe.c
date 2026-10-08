#include "asm.h"

#include "types.h"
#include "rnc/sdk/libmpeg.h"
extern s32 _dispRefImage(struct MpegDecoder *mpeg, struct MpegRefImage *arg1, s32 arg2);
extern s32 _dispRefImageField(struct MpegDecoder *mpeg, struct MpegRefImage *arg1, struct MpegRefImage *arg2, s32 arg3);
void _outputFrame(struct MpegDecoder *mpeg, s32 arg1, s32 arg2) {
    struct MpegRefImage *var_5_14;
    struct MpegRefImage *var_5_24;
    struct MpegRefImage *var_6_27;

    if (arg2 != 0) {
        if (mpeg->picture_structure == 3) {
            if (mpeg->picture_coding_type == 3) {
                var_5_14 = mpeg->ref_images[3];
            } else {
                var_5_14 = mpeg->ref_images[0];
            }
            _dispRefImage(mpeg, var_5_14, arg1 - 1);
        } else {
            if (mpeg->picture_coding_type == 3) {
                var_5_24 = mpeg->ref_images[7];
                var_6_27 = mpeg->ref_images[11];
            } else {
                var_5_24 = mpeg->ref_images[4];
                var_6_27 = mpeg->ref_images[8];
            }
            _dispRefImageField(mpeg, var_5_24, var_6_27, arg1 - 1);
        }
    }
    if (mpeg->unkF8 == 1) {
        mpeg->unkF8 = 2;
    }
}
