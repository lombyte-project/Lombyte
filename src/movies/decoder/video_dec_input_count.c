#include "types.h"
#include "rnc/movies/decoder/video_dec.h"

extern s32 vi_buf_count(struct ViBuf *buf) __asm__("FUN_0023c610");
s32        video_dec_input_count(struct VideoDec *video_dec) __asm__("FUN_0023cce0");

s32 video_dec_input_count(struct VideoDec *video_dec) {
    return vi_buf_count(&video_dec->vi_buf);
}

extern s32 func_0023CCE0(struct VideoDec *video_dec) __attribute__((alias("FUN_0023cce0")));
