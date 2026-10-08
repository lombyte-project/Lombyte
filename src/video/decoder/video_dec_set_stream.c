#include "types.h"
#include "rnc/video/decoder/video_dec.h"

/* Retail 0x0023cbd0 forwards all five incoming arguments unchanged to
   sceMpegAddStrCallback.
   init_all installs video (type/channel 0/0) and PCM (type 3) callbacks,
   each with the decoder context as user data. The wrapper ignores the
   SDK's previous-callback result and always returns 1; this is not a
   checked registration-success result. Explicit forwarding is required
   in C, even though EE argument registers happen to survive the old call. */
s32 video_dec_set_stream(struct VideoDec *video_dec, s32 stream_type,
                         s32 channel, MpegStreamCallback callback,
                         void *callback_context) __asm__("FUN_0023cbd0");

s32 video_dec_set_stream(struct VideoDec *video_dec, s32 stream_type,
                         s32 channel, MpegStreamCallback callback,
                         void *callback_context) {
    sceMpegAddStrCallback(&video_dec->mpeg, stream_type, channel, callback, callback_context);
    return 1;
}

/* Recovered original symbol name. */
extern __typeof__(video_dec_set_stream)
    videoDecSetStream__FP8VideoDeciiPFP7sceMpegP13sceMpegCbDataPv_iPv
    __attribute__((alias("FUN_0023cbd0")));
