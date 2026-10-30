#include "types.h"
#include "rnc/sdk/libmpeg.h"

typedef struct MpegDecoder MpegDecoder;
extern void                FinishMpegFrame(MpegDecoder *decoder) __asm__("_lastFrame");

s32 _sceMpegFlush(struct sceMpeg *mpeg) __asm__("_sceMpegFlush");

s32 _sceMpegFlush(struct sceMpeg *mpeg) {
    MpegDecoder *decoder = mpeg->sys;
    s32          result = 0;

    if (decoder->decoded_pictures != 0) {
        if (decoder->started != 0) {
            FinishMpegFrame(decoder);
            mpeg->frameCount = decoder->frame_count - decoder->frame_base;
            decoder->decoded_pictures = 0;
            result = 1;
        }
    }
    return result;
}
