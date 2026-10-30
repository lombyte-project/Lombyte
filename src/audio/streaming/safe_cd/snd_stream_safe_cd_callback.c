#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/audio/sound_state.h"

extern s32 sceCdCallback();
s32        snd_stream_safe_cd_callback(s32 callback) __asm__("FUN_0012ef28");

s32 snd_stream_safe_cd_callback(s32 callback) {
    s32 prev_callback;
    if (snd_cd_stream_active == 0) {
        return sceCdCallback(callback);
    }
    prev_callback = snd_cd_status_callback;
    snd_cd_status_callback = callback;
    return prev_callback;
}
