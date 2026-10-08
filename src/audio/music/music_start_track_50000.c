#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/globals.h"
#include "rnc/storage/disc_table.h"

extern u8 D_002169C0[];
extern s32 snd_play_vag_stream_by_loc_ex_cb() __asm__("func_0012EC08");

void music_start_track_50000(s32 track, s32 flags, s32 volume) __asm__("FUN_00215518");

void music_start_track_50000(s32 track, s32 flags, s32 volume) {
    s32 handle;

    handle = disc_table.music_50000[track - 50000][game_language];
    if (handle != 0) {
        if (music_stream_state.secondary.handle == 0) {
            *(u32 *)&music_stream_state.secondary.handle = 0xFFFFFFFF;
            music_stream_state.secondary.state = 1;
            music_stream_state.secondary.track = track;
            music_stream_state.secondary.flags = flags;
            music_stream_state.secondary.poll_interval = 10;
            music_stream_state.secondary.remaining_time = 48000;
            music_stream_state.secondary.volume = volume;
            music_stream_state.secondary.crossfade_enabled = 0;
            snd_play_vag_stream_by_loc_ex_cb(
                handle, 0, 0, 0, (s16)volume, 0, 2, 0, 0x21, D_002169C0,
                (u64)((s64)(((u8 *)&music_stream_state + 0x50)) << 0x20) >> 0x20);
        }
    }
}

extern __typeof__(music_start_track_50000) func_00215518 __attribute__((alias("FUN_00215518")));
