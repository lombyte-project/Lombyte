#include "types.h"
#include "asm.h"

#include "rnc/audio/music/music_stream_state.h"

extern s32 snd_stream_safe_cd_get_error() __asm__("func_0012EEF0");
void finish_audio_stream_read(s32 arg0) __asm__("FUN_00216950");

void finish_audio_stream_read(s32 arg0) {
    if (arg0 == 1) {
        music_stream_state.read_state = 0;
        if (snd_stream_safe_cd_get_error() != 0) {
            music_stream_state.read_state = 2;
        }
    }
}
