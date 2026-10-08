#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/storage/disc_table.h"

extern void func_00216A20();
extern void snd_play_vag_stream_by_loc_ex_cb(s32, s32, s32, s32, s16, s32, s32, s32, s32,
                                             void (*)(), u64) __asm__("func_0012EC08");

void music_preseek_track(s32 track, s32 track_flags, s32 volume) __asm__("FUN_00215b68");

void music_preseek_track(s32 track, s32 track_flags, s32 volume) {
    volatile s32 *entry;
    s64 v;
    u8 *tbl;
    s32 off;

    if (music_stream_state.primary.handle == 0) {
        tbl = (u8 *)&disc_table;
        off = 0x2AA8;
        entry = (s32 *)(tbl + off) + track;
        if (*entry != 0) {
            *(u32 *)&music_stream_state.primary.handle = 0xFFFFFFFF;
            music_stream_state.primary.state = 1;
            music_stream_state.primary.track = track;
            music_stream_state.primary.flags = track_flags;
            music_stream_state.primary.poll_interval = 10;
            music_stream_state.primary.remaining_time = 0xBB80;
            music_stream_state.primary.volume = volume;
            music_stream_state.primary.crossfade_enabled = 0;
            v = *entry;
            snd_play_vag_stream_by_loc_ex_cb(v, 0, 0, 0, volume, 0, 1, 0, 0x21, func_00216A20,
                                             (u64)((s64)(((u8 *)&music_stream_state + 0x34)) << 0x20) >>
                                                 0x20);
        }
    }
}

extern __typeof__(music_preseek_track) func_00215B68 __attribute__((alias("FUN_00215b68")));
