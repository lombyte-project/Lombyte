#include "types.h"

extern s32 snd_close_movie_sound() __asm__("FUN_0012f0e0");
s32        audio_dec_delete(s32 unused_arg) __asm__("FUN_0023ac90");

s32 audio_dec_delete(s32 unused_arg) {
    snd_close_movie_sound();
    return 1;
}

extern s32 func_0023AC90(s32 arg0) __attribute__((alias("FUN_0023ac90")));
