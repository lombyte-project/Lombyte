#include "types.h"
#include "asm.h"

#include "rnc/audio/music/music_stream_state.h"

extern s32 snd_StreamSafeCdGetError() __asm__("func_0012EEF0");
void finish_audio_stream_read(s32 callback_value) __asm__("FUN_00216950");

void finish_audio_stream_read(s32 callback_value) {
    if (callback_value == 1) {
        music_stream_state.read_state = 0;
        if (snd_StreamSafeCdGetError() != 0) {
            music_stream_state.read_state = 2;
        }
    }
}
