#include "types.h"
extern s32 FUN_0012b9e0();
extern s32 vi_buf_delete() __asm__("FUN_0023c5b8");
s32 video_dec_delete(s32 video_dec) __asm__("FUN_0023cc38");

s32 video_dec_delete(s32 video_dec) {
    vi_buf_delete(video_dec + 0x48);
    FUN_0012b9e0(video_dec);
    return 1;
}

extern s32 func_0023CC38(s32 video_dec) __attribute__((alias("FUN_0023cc38")));
