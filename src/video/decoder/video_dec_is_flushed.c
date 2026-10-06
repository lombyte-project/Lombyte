#include "types.h"
extern s32 func_0012BA58();
extern s32 video_dec_input_count(s32 video_dec) __asm__("func_0023CCE0");
s32 video_dec_is_flushed(s32 video_dec) __asm__("FUN_0023cde0");

s32 video_dec_is_flushed(s32 video_dec) {
    s32 flushed;

    flushed = 0;
    if (video_dec_input_count(video_dec) == 0) {
        flushed = func_0012BA58(video_dec) != 0;
    }
    return flushed;
}

/* Recovered original symbol name. */
extern __typeof__(video_dec_is_flushed) videoDecIsFlushed__FP8VideoDec
    __attribute__((alias("FUN_0023cde0")));
