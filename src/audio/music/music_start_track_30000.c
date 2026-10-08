#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/globals.h"
extern s32 D_0011C178[];
extern u8 D_0015EE1C;
extern u8 D_00151720[];
extern void func_002169C0();
extern void snd_play_vag_stream_by_loc_ex_cb(s32, s32, s32, s32, s16, s32, s32, s32, s32,
                                             void (*)(), u64) __asm__("func_0012EC08");
void music_start_track_30000(s32 track, s32 flags, s32 volume) __asm__("FUN_002156d8");

void music_start_track_30000(s32 track, s32 flags, s32 volume) {
    s32 *entry;

    entry = (s32 *)((u8 *)D_0011C178 + (track * 4 + game_language * 600));
    if (*entry != 0 && music_stream_state.secondary.handle == 0) {
        *(u32 *)&music_stream_state.secondary.handle = 0xFFFFFFFF;
        music_stream_state.secondary.state = 1;
        music_stream_state.secondary.track = track;
        music_stream_state.secondary.flags = flags;
        music_stream_state.secondary.poll_interval = 10;
        music_stream_state.secondary.remaining_time = 48000;
        music_stream_state.secondary.volume = volume;
        music_stream_state.secondary.crossfade_enabled = 0;
        snd_play_vag_stream_by_loc_ex_cb(*entry, 0, 0, 0, D_0015EE1C ? volume : 0, 0, 2, 0, 0x21,
                                         func_002169C0, (u32)D_00151720);
    }
}

extern __typeof__(music_start_track_30000) func_002156D8 __attribute__((alias("FUN_002156d8")));
