#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/storage/disc_table.h"

// D_002169C0 is a code address retail passes as a pointer, not a data symbol;
// config/us/undefined_syms.txt binds its absolute value so this extern links.
extern u8 D_002169C0[];
extern s32 snd_play_vag_stream_by_loc_ex_cb() __asm__("func_0012EC08");

void music_start_track_60000(s32 track, s32 track_flags, s32 volume) __asm__("FUN_00215440");

void music_start_track_60000(s32 track, s32 track_flags, s32 volume) {
    s32 location;

    location = disc_table.music_60000[track - 60000].sector;
    if (location != 0) {
        if (music_stream_state.secondary.handle == 0) {
            *(u32 *)&music_stream_state.secondary.handle = 0xFFFFFFFF;
            music_stream_state.secondary.state = 1;
            music_stream_state.secondary.track = track;
            music_stream_state.secondary.flags = track_flags;
            music_stream_state.secondary.poll_interval = 10;
            music_stream_state.secondary.remaining_time = 48000;
            music_stream_state.secondary.volume = volume;
            music_stream_state.secondary.crossfade_enabled = 0;
            snd_play_vag_stream_by_loc_ex_cb(
                location, 0, 0, 0, (s16)volume, 0, 2, 0, 0x21, D_002169C0,
                (u64)((s64)(((u8 *)&music_stream_state + 0x50)) << 0x20) >> 0x20);
        }
    }
}

extern __typeof__(music_start_track_60000) func_00215440 __attribute__((alias("FUN_00215440")));
