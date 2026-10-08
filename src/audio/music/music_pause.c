#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
void music_pause(s32 arg0) {
    if (arg0 != 0) {
        music_stream_state.secondary_fade_flags = -0x8000;
        music_stream_state.unk5E = 0;
    }
    music_stream_state.primary_fade_flags = -0x8000;
    music_stream_state.unk42 = 0;
    music_stream_state.transition_fade_flags = -0x8000;
    music_stream_state.unk7A = 0;
}

/* ACCEPTED: attempt-1 (sn-O2) direct 100/100/100; s16 fields at 0x40/0x42/0x5C/0x5E/0x78/0x7A, retail store order. */
