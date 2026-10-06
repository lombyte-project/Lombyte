#include "types.h"
extern s32 FUN_0012aec8();
s32 video_dec_set_stream(s32 arg0) __asm__("FUN_0023cbd0");

s32 video_dec_set_stream(s32 arg0) {
    FUN_0012aec8();
    return 1;
}

/* Recovered original symbol name. */
extern __typeof__(video_dec_set_stream)
    videoDecSetStream__FP8VideoDeciiPFP7sceMpegP13sceMpegCbDataPv_iPv
    __attribute__((alias("FUN_0023cbd0")));
