#include "types.h"
extern u8 D_00137B00[];
extern s32 D_0015EC8C;
extern s32 sceCdGetError();
s32 snd_stream_safe_cd_get_error(s32 arg0) __asm__("FUN_0012eef0");

s32 snd_stream_safe_cd_get_error(s32 arg0) {
    u8 *p;
    if (D_0015EC8C == 0) {
        return sceCdGetError();
    }
    p = D_00137B00;
    return *(volatile s32 *)(p + 0x10);
}

extern __typeof__(snd_stream_safe_cd_get_error) func_0012EEF0
    __attribute__((alias("FUN_0012eef0")));

/* Recovered original symbol name. */
extern __typeof__(snd_stream_safe_cd_get_error) snd_StreamSafeCdGetError
    __attribute__((alias("FUN_0012eef0")));
