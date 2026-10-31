#include "types.h"

extern s32  snd_stream_safe_cd_callback() __asm__("FUN_0012ef28");
extern void finish_audio_stream_read() __asm__("FUN_00216950");
void        register_audio_stream_callback(void) __asm__("FUN_00215420");

/* Moby class 1750 */
void register_audio_stream_callback(void) {
    snd_stream_safe_cd_callback(&finish_audio_stream_read);
}

extern void func_00215420(void) __attribute__((alias("FUN_00215420")));
