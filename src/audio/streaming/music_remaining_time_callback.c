/* Ported from rac1-decomp (src/game/stream.c, func_00217A60). */
#include "types.h"
#include "rnc/audio/music/music_stream_state.h"

void music_remaining_time_callback(int remaining_time, long context) __asm__("FUN_00216bc0");

/* Stream callback: stores the channel's remaining time; with crossfade
   enabled on the channel and crossfade_state 1, moves crossfade_state to 2
   and copies the time (and a quarter of it) into the crossfade fields. */
void music_remaining_time_callback(int remaining_time, long context) {
    struct MusicStreamChannel *channel = (struct MusicStreamChannel *)(int)context;
    struct MusicStreamState *music_state;
    if (channel == 0) {
        return;
    }
    channel->remaining_time = remaining_time;
    if (channel->crossfade_enabled == 0) {
        return;
    }
    music_state = &music_stream_state;
    if (music_state->crossfade_state != 1) {
        return;
    }
    if (remaining_time == 0) {
        return;
    }
    music_state->crossfade_state = 2;
    music_state->crossfade_remaining_time = channel->remaining_time;
    music_state->crossfade_interval = channel->remaining_time / 4;
}

extern __typeof__(music_remaining_time_callback) func_00216BC0
    __attribute__((alias("FUN_00216bc0")));
