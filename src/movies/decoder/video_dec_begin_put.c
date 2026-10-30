#include "types.h"
extern s32 vi_buf_begin_put() __asm__("FUN_0023be20");
void video_dec_begin_put(s32 video_dec) __asm__("FUN_0023cbf0");

void video_dec_begin_put(s32 video_dec) {
    vi_buf_begin_put(video_dec + 0x48);
}

extern void func_0023CBF0(s32 video_dec) __attribute__((alias("FUN_0023cbf0")));
