#include "types.h"

#include "rnc/audio/music/music_stream_state.h"

extern s32 snd_flush_sound_commands() __asm__("func_0012DC80");
extern s32 snd_stop_all_streams() __asm__("func_0012EBD0");
extern s32 snd_stream_safe_check_cd_idle() __asm__("func_0012ED30");
void music_stop(void) __asm__("FUN_00215ee8");

void music_stop(void) {
    s32 cur;

    if (music_stream_state.primary_handle == 0xFFFFFFFF) {
        do {
            snd_flush_sound_commands();
        } while (music_stream_state.primary_handle == 0xFFFFFFFF);
    }
    if (music_stream_state.transition_handle == 0xFFFFFFFF) {
        do {
            snd_flush_sound_commands();
        } while (music_stream_state.transition_handle == 0xFFFFFFFF);
    }
    if (music_stream_state.secondary_handle == 0xFFFFFFFF) {
        do {
            snd_flush_sound_commands();
        } while (music_stream_state.secondary_handle == 0xFFFFFFFF);
    }
    snd_stop_all_streams();
    do {

    } while (snd_flush_sound_commands() != 0);
    snd_stream_safe_check_cd_idle(1);
    cur = music_stream_state.requested_track;
    music_stream_state.primary_state = 0;
    music_stream_state.primary_flags = 0;
    music_stream_state.primary_handle = 0;
    if (cur != -1) {
        music_stream_state.primary_track = cur;
    }
    music_stream_state.secondary_state = 0;
    music_stream_state.secondary_flags = 0;
    music_stream_state.secondary_handle = 0;
    music_stream_state.transition_state = 0;
    music_stream_state.transition_flags = 0;
    music_stream_state.transition_handle = 0;
    music_stream_state.crossfade_state = 0;
    music_stream_state.requested_track = -1;
    music_stream_state.requested_transition_track = -1;
}

extern __typeof__(music_stop) func_00215EE8 __attribute__((alias("FUN_00215ee8")));
