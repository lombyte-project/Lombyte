#include "types.h"
#include "rnc/audio/music/music_stream_state.h"

void music_pause(s32 pause_secondary) {
    if (pause_secondary != 0) {
        music_stream_state.secondary.pause_flags = -0x8000;
        music_stream_state.secondary.pause_timer = 0;
    }
    music_stream_state.primary.pause_flags = -0x8000;
    music_stream_state.primary.pause_timer = 0;
    music_stream_state.transition.pause_flags = -0x8000;
    music_stream_state.transition.pause_timer = 0;
}

/* ACCEPTED: attempt-1 (sn-O2) direct 100/100/100; s16 fields at 0x40/0x42/0x5C/0x5E/0x78/0x7A, retail store order. */
