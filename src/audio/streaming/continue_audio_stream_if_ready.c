#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
extern s32 snd_continue_vag_stream() __asm__("func_0012ECA0");
s32 continue_audio_stream_if_ready(void) __asm__("FUN_00215b10");

s32 continue_audio_stream_if_ready(void) {
    if (music_stream_state.secondary.handle != 0) {
        if (music_stream_state.secondary.state == 3) {
            snd_continue_vag_stream(music_stream_state.secondary.handle);
            music_stream_state.secondary.state = 4;
            return 1;
        }
        return 0;
    }
    return 0;
}
