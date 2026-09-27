#include "types.h"
extern s32 D_0015EC8C;
extern s32 D_0015EC90;
extern s32 sceCdCallback();
s32 snd_stream_safe_cd_callback(s32 arg0) __asm__("FUN_0012ef28");

s32 snd_stream_safe_cd_callback(s32 arg0) {
    s32 temp_2_9;
    if (D_0015EC8C == 0) {
        return sceCdCallback(arg0);
    }
    temp_2_9 = D_0015EC90;
    D_0015EC90 = arg0;
    return temp_2_9;
}
