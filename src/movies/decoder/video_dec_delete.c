#include "types.h"
#include "rnc/sdk/libmpeg.h"

extern s32 sceMpegDelete(struct sceMpeg *mpeg) __asm__("FUN_0012b9e0");
extern s32 vi_buf_delete() __asm__("FUN_0023c5b8");
s32        video_dec_delete(s32 video_dec) __asm__("FUN_0023cc38");

/* Mirrors video_dec_create: the sceMpeg sits at the start of the decoder. */
s32 video_dec_delete(s32 video_dec) {
    vi_buf_delete(video_dec + 0x48);
    sceMpegDelete((struct sceMpeg *)video_dec);
    return 1;
}

extern s32 func_0023CC38(s32 video_dec) __attribute__((alias("FUN_0023cc38")));
