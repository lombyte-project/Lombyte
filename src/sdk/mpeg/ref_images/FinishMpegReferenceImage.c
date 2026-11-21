/* Ported from rac1-decomp (src/core/00125630.c, func_00129C78). */
#include "types.h"
#include "rnc/sdk/libmpeg.h"

/* Marks a picture as output. The first one also restarts sceMpeg.frameCount
   (frame_base = frame_count). */
int FinishMpegReferenceImage(void *object) __asm__("FUN_00129b38");

int FinishMpegReferenceImage(void *mpeg_reference_image) {
    struct MpegDecoder *decoder = (struct MpegDecoder *)mpeg_reference_image;
    int                 return_value = 1;
    if (decoder->started != 2) {
        int frame_count = decoder->frame_count;
        decoder->started = 2;
        decoder->frame_base = frame_count;
    }
    decoder->picture_ready = return_value;
    return return_value;
}

extern __typeof__(FinishMpegReferenceImage) func_00129B38 __attribute__((alias("FUN_00129b38")));
