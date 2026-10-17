#include "types.h"
#include "rnc/audio/sound_read_work.h"
#include "rnc/data_labels.h"
#include "rnc/audio/sound_state.h"
extern s32 sceCdGetError();
s32 snd_stream_safe_cd_get_error(s32 arg0) __asm__("FUN_0012eef0");

s32 snd_stream_safe_cd_get_error(s32 unused_arg) {
    if (snd_cd_stream_active == 0) {
        return sceCdGetError();
    }
    return sound_read_work.read_error;
}

extern __typeof__(snd_stream_safe_cd_get_error) func_0012EEF0
    __attribute__((alias("FUN_0012eef0")));

/* Recovered original symbol name. */
extern __typeof__(snd_stream_safe_cd_get_error) snd_StreamSafeCdGetError
    __attribute__((alias("FUN_0012eef0")));
