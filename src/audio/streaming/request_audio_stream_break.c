#include "types.h"
#include "asm.h"

#include "rnc/audio/music/music_stream_state.h"

extern s32 snd_stream_safe_cd_break() __asm__("func_0012EEA8");
void       request_audio_stream_break(s32 unused_parameter) __asm__("FUN_002166e8");

void request_audio_stream_break(s32 unused_parameter) {
    enum {
        MUSIC_STREAM_READ_INACTIVE = 0
    };
    if (music_stream_state.read_state != MUSIC_STREAM_READ_INACTIVE) {
        snd_stream_safe_cd_break();
        music_stream_state.break_requested = 1;
    }
}
