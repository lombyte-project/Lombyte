#include "types.h"
#include "rnc/audio/music/music_stream_state.h"

extern u8 D_0013A664[];
extern s32 D_0015ED88;
// D_002169C0 is a code address retail passes as a pointer, not a data symbol;
// config/us/undefined_syms.txt binds its absolute value so this extern links.
extern u8 D_002169C0[];
extern struct MusicStreamState D_001516D0;
extern s32 snd_play_vag_stream_by_loc_ex_cb() __asm__("func_0012EC08");
extern void music_start_track_60000(s32, s32, s32) __asm__("FUN_00215440");
extern void music_start_track_50000(s32, s32, s32) __asm__("FUN_00215518");
extern void music_start_track_40000(s32, s32, s32) __asm__("FUN_00215600");
extern void music_start_track_30000(s32, s32, s32) __asm__("FUN_002156d8");
extern void music_start_track_20000(s32, s32, s32) __asm__("FUN_002157d0");
extern void music_start_track_10000(s32, s32, s32) __asm__("FUN_002158a0");

void music_start_track_by_id(s32 track, s32 track_flags, s32 volume) __asm__("FUN_00215970");

void music_start_track_by_id(s32 track, s32 track_flags, s32 volume) {
    s32 handle;

    if (track > 0xEA5F) {
        music_start_track_60000(track, track_flags, volume);
    } else if (track > 0xC34F) {
        music_start_track_50000(track, track_flags, volume);
    } else if (track > 0x9C3F) {
        music_start_track_40000(track, track_flags, volume);
    } else if (track >= 0x7530) {
        music_start_track_30000(track, track_flags, volume);
    } else if (track >= 0x4E20) {
        music_start_track_20000(track, track_flags, volume);
    } else if (track >= 0x2710) {
        music_start_track_10000(track, track_flags, volume);
    } else {
        handle = *((s32 *)((u8 *)D_0013A664 + track * 0x250) + D_0015ED88);
        if (handle != 0) {
            if (D_001516D0.secondary_handle == 0) {
                *(u32 *)&D_001516D0.secondary_handle = 0xFFFFFFFF;
                D_001516D0.secondary_state = 1;
                D_001516D0.secondary_track = track;
                D_001516D0.secondary_flags = track_flags;
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
}

extern __typeof__(music_start_track_by_id) func_00215970 __attribute__((alias("FUN_00215970")));
