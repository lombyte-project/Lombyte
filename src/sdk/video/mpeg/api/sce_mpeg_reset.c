#include "types.h"
#include "rnc/sdk/libmpeg.h"

typedef struct MpegDecoder MpegDecoder;
extern void                ClearDecoderState(MpegDecoder *decoder) __asm__("_clearEach");
extern void                set_mpeg1_mode(MpegDecoder *decoder) __asm__("SetImageBufferFlag");

void sceMpegReset(struct sceMpeg *mpeg) __asm__("sceMpegReset");

void sceMpegReset(struct sceMpeg *mpeg) {
    MpegDecoder *decoder = mpeg->sys;

    decoder->is_end = 0;
    decoder->decoded_pictures = 0;
    decoder->started = 0;
    mpeg->frameCount = 0;
    decoder->frame_base = 0;
    decoder->unk80 = -1;
    ClearDecoderState(decoder);
    decoder->frame_count = 0;
    set_mpeg1_mode(decoder);
}
