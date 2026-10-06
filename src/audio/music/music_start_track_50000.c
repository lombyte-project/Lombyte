#include "types.h"
#include "rnc/audio/music/music_stream_state.h"

struct Tracks {
    u8 pad[0xF00];
    s32 h[1][6];
};
extern struct Tracks D_00137B80;
extern s32 D_0015ED88;
extern struct MusicStreamState D_001516D0;
extern u8 D_002169C0[];
extern s32 snd_play_vag_stream_by_loc_ex_cb() __asm__("func_0012EC08");

void music_start_track_50000(s32 track, s32 flags, s32 volume) __asm__("FUN_00215518");

void music_start_track_50000(s32 track, s32 flags, s32 volume) {
    s32 handle;

    handle = D_00137B80.h[track - 50000][D_0015ED88];
    if (handle != 0) {
        if (D_001516D0.secondary_handle == 0) {
            *(u32 *)&D_001516D0.secondary_handle = 0xFFFFFFFF;
            D_001516D0.secondary_state = 1;
            D_001516D0.secondary_track = track;
            D_001516D0.secondary_flags = flags;
            D_001516D0.secondary_poll_interval = 10;
            D_001516D0.secondary_remaining_time = 48000;
            D_001516D0.secondary_volume = volume;
            D_001516D0.secondary_crossfade_enabled = 0;
            snd_play_vag_stream_by_loc_ex_cb(
                handle, 0, 0, 0, (s16)volume, 0, 2, 0, 0x21, D_002169C0,
                (u64)((s64)(((u8 *)&D_001516D0 + 0x50)) << 0x20) >> 0x20);
        }
    }
}

extern __typeof__(music_start_track_50000) func_00215518 __attribute__((alias("FUN_00215518")));
