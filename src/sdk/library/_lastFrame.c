#include "types.h"
#include "rnc/sdk/libmpeg.h"
extern u8 D_00153AB8[];
extern s32 _Error(struct MpegDecoder *, u8 *, s32);
extern s32 _dispRefImage(struct MpegDecoder *, struct MpegRefImage *, s32);
extern s32 _dispRefImageField(struct MpegDecoder *, struct MpegRefImage *, struct MpegRefImage *, s32);

void _lastFrame(struct MpegDecoder *decoder) {
    s32 count;
    count = decoder->frame_count;
    if (decoder->second_field != 0) {
        _Error(decoder, D_00153AB8, count);
    } else {
        if (decoder->picture_structure == 3) {
            _dispRefImage(decoder, decoder->ref_images[1], count - 1);
        } else {
            _dispRefImageField(decoder, decoder->ref_images[5], decoder->ref_images[9], count - 1);
        }
    }
    decoder->second_field = 0;
}
