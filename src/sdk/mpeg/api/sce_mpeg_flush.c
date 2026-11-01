#include "types.h"
#include "rnc/sdk/libmpeg.h"
#include "rnc/sdk/mpeg/picture.h"

typedef struct MpegDecoder MpegDecoder;

s32 _sceMpegFlush(struct sceMpeg *mpeg) __asm__("_sceMpegFlush");

s32 _sceMpegFlush(struct sceMpeg *mpeg) {
    MpegDecoder *decoder = mpeg->sys;
    s32          frame_flushed = 0;

    if (decoder->decoded_pictures != 0) {
        if (decoder->started != 0) {
            _lastFrame(decoder);
            mpeg->frameCount = decoder->frame_count - decoder->frame_base;
            decoder->decoded_pictures = 0;
            frame_flushed = 1;
        }
    }
    return frame_flushed;
}
