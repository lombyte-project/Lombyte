#include "types.h"
extern s32 vi_buf_count() __asm__("FUN_0023c610");
void video_dec_input_count(s32 video_dec) __asm__("FUN_0023cce0");

void video_dec_input_count(s32 video_dec) {
    vi_buf_count(video_dec + 0x48);
}

extern void func_0023CCE0(s32 video_dec) __attribute__((alias("FUN_0023cce0")));
