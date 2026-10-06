#include "types.h"
extern s32 func_0012BA58();
extern s32 video_dec_input_count(s32 arg0) __asm__("func_0023CCE0");
s32 video_dec_is_flushed(s32 arg0) __asm__("FUN_0023cde0");

s32 video_dec_is_flushed(s32 arg0) {
    s32 var_2_13;

    var_2_13 = 0;
    if (video_dec_input_count(arg0) == 0) {
        var_2_13 = func_0012BA58(arg0) != 0;
    }
    return var_2_13;
}

/* Recovered original symbol name. */
extern __typeof__(video_dec_is_flushed) videoDecIsFlushed__FP8VideoDec __attribute__((alias("FUN_0023cde0")));
