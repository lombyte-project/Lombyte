#include "types.h"
#include "rnc/movies/decoder/video_dec.h"

extern s32 sceMpegIsRefBuffEmpty(struct sceMpeg *mpeg) __asm__("func_0012BA58");
extern s32 video_dec_input_count(struct VideoDec *video_dec) __asm__("func_0023CCE0");
s32        video_dec_is_flushed(struct VideoDec *video_dec) __asm__("FUN_0023cde0");

s32 video_dec_is_flushed(struct VideoDec *video_dec) {
    s32 flushed;

    flushed = 0;
    if (video_dec_input_count(video_dec) == 0) {
        flushed = sceMpegIsRefBuffEmpty(&video_dec->mpeg) != 0;
    }
    return flushed;
}

/* Recovered original symbol name. */
extern __typeof__(video_dec_is_flushed) videoDecIsFlushed__FP8VideoDec
    __attribute__((alias("FUN_0023cde0")));
