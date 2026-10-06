#include "types.h"
#include "rnc/audio/music/music_stream_state.h"

struct MusicTable {
    u8 pad0[0x2AA8];
    s32 tracks[1];
};

extern struct MusicTable D_00137B80;
extern struct MusicStreamState D_001516D0;
extern void func_00216A20();
extern void snd_play_vag_stream_by_loc_ex_cb(s32, s32, s32, s32, s16, s32, s32, s32, s32,
                                             void (*)(), u64) __asm__("func_0012EC08");

void music_preseek_track(s32 track, s32 track_flags, s32 volume) __asm__("FUN_00215b68");

void music_preseek_track(s32 track, s32 track_flags, s32 volume) {
    volatile s32 *entry;
    s64 v;
    u8 *tbl;
    s32 off;

    if (D_001516D0.primary_handle == 0) {
        tbl = (u8 *)&D_00137B80;
        off = 0x2AA8;
        entry = (s32 *)(tbl + off) + track;
        if (*entry != 0) {
            *(u32 *)&D_001516D0.primary_handle = 0xFFFFFFFF;
            D_001516D0.primary_state = 1;
            D_001516D0.primary_track = track;
            D_001516D0.primary_flags = track_flags;
            D_001516D0.primary_poll_interval = 10;
            D_001516D0.primary_remaining_time = 0xBB80;
            D_001516D0.primary_volume = volume;
            D_001516D0.primary_crossfade_enabled = 0;
            v = *entry;
            snd_play_vag_stream_by_loc_ex_cb(v, 0, 0, 0, volume, 0, 1, 0, 0x21, func_00216A20,
                                             (u64)((s64)(((u8 *)&D_001516D0 + 0x34)) << 0x20) >>
                                                 0x20);
        }
    }
}

extern __typeof__(music_preseek_track) func_00215B68 __attribute__((alias("FUN_00215b68")));
