#include "types.h"

#include "rnc/audio/music/music_stream_state.h"

extern s32 snd_flush_sound_commands() __asm__("func_0012DC80");
extern s32 snd_stop_all_streams() __asm__("func_0012EBD0");
extern s32 snd_stream_safe_check_cd_idle() __asm__("func_0012ED30");
void music_stop(void) __asm__("FUN_00215ee8");

void music_stop(void) {
    s32 cur;

    if (music_stream_state.primary.handle == 0xFFFFFFFF) {
        do {
            snd_flush_sound_commands();
        } while (music_stream_state.primary.handle == 0xFFFFFFFF);
    }
    if (music_stream_state.transition.handle == 0xFFFFFFFF) {
        do {
            snd_flush_sound_commands();
        } while (music_stream_state.transition.handle == 0xFFFFFFFF);
    }
    if (music_stream_state.secondary.handle == 0xFFFFFFFF) {
        do {
            snd_flush_sound_commands();
        } while (music_stream_state.secondary.handle == 0xFFFFFFFF);
    }
    snd_stop_all_streams();
    do {

    } while (snd_flush_sound_commands() != 0);
    snd_stream_safe_check_cd_idle(1);
    cur = music_stream_state.requested_track;
    music_stream_state.primary.state = 0;
    music_stream_state.primary.flags = 0;
    music_stream_state.primary.handle = 0;
    if (cur != -1) {
        music_stream_state.primary.track = cur;
    }
    music_stream_state.secondary.state = 0;
    music_stream_state.secondary.flags = 0;
    music_stream_state.secondary.handle = 0;
    music_stream_state.transition.state = 0;
    music_stream_state.transition.flags = 0;
    music_stream_state.transition.handle = 0;
    music_stream_state.crossfade_state = 0;
    music_stream_state.requested_track = -1;
    music_stream_state.requested_transition_track = -1;
}

extern __typeof__(music_stop) func_00215EE8 __attribute__((alias("FUN_00215ee8")));
