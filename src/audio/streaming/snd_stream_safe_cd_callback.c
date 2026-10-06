#include "types.h"
extern s32 D_0015EC8C;
extern s32 D_0015EC90;
extern s32 sceCdCallback();
s32 snd_stream_safe_cd_callback(s32 callback) __asm__("FUN_0012ef28");

s32 snd_stream_safe_cd_callback(s32 callback) {
    s32 prev_callback;
    if (D_0015EC8C == 0) {
        return sceCdCallback(callback);
    }
    prev_callback = D_0015EC90;
    D_0015EC90 = callback;
    return prev_callback;
}
