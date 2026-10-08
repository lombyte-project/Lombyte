#include "types.h"
#include "rnc/sdk/libmpeg.h"
extern u8 D_00153AB8[];
extern s32 _Error(struct MpegDecoder *, u8 *, s32);
extern s32 _dispRefImage(struct MpegDecoder *, s32, s32);
extern s32 _dispRefImageField(struct MpegDecoder *, s32, s32, s32);

void _lastFrame(struct MpegDecoder *arg0) {
    s32 count;
    count = arg0->frame_count;
    if (arg0->unk120 != 0) {
        _Error(arg0, D_00153AB8, count);
    } else {
        if (arg0->picture_structure == 3) {
            _dispRefImage(arg0, arg0->unk1BC, count - 1);
        } else {
            _dispRefImageField(arg0, arg0->unk1CC, arg0->unk1DC, count - 1);
        }
    }
    arg0->unk120 = 0;
}
