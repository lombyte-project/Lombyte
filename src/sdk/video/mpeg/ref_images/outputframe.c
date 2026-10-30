#include "asm.h"

#include "types.h"
#include "rnc/sdk/libmpeg.h"

extern s32 _dispRefImage(struct MpegDecoder *mpeg, struct MpegRefImage *arg1, s32 arg2);
extern s32 _dispRefImageField(struct MpegDecoder  *mpeg,
                              struct MpegRefImage *arg1,
                              struct MpegRefImage *arg2,
                              s32                  arg3);

void _outputFrame(struct MpegDecoder *mpeg, s32 frame_counter, s32 output_enabled) {
    struct MpegRefImage *frame_image;
    struct MpegRefImage *top_field;
    struct MpegRefImage *bottom_field;

    if (output_enabled != 0) {
        if (mpeg->picture_structure == 3) {
            if (mpeg->picture_coding_type == 3) {
                frame_image = mpeg->ref_images[3];
            } else {
                frame_image = mpeg->ref_images[0];
            }
            _dispRefImage(mpeg, frame_image, frame_counter - 1);
        } else {
            if (mpeg->picture_coding_type == 3) {
                top_field = mpeg->ref_images[7];
                bottom_field = mpeg->ref_images[11];
            } else {
                top_field = mpeg->ref_images[4];
                bottom_field = mpeg->ref_images[8];
            }
            _dispRefImageField(mpeg, top_field, bottom_field, frame_counter - 1);
        }
    }
    if (mpeg->unkF8 == 1) {
        mpeg->unkF8 = 2;
    }
}
