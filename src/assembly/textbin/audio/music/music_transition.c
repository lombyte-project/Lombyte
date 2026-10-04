#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/audio/music/music_transition/FUN_00215e00.s", FUN_00215e00);
#else
#include "types.h"
#include "rnc/music_stream_state.h"
struct MusicTable { u8 pad0[0x2AA8]; s32 tracks[1]; };
extern struct MusicTable level_music_table __asm__("D_00137B80");
extern struct MusicStreamState music_state __asm__("D_001516D0");
extern void music_transition_start_callback() __asm__("FUN_00216a80");
extern void snd_play_vag_stream_by_loc_ex_cb(s32, s32, s32, s32, s16, s32, s32, s32, s32, void (*)(), u64) __asm__("func_0012EC08");
s32 music_transition(s32 target_track, s32 transition_track, s32 flags, s32 volume) __asm__("FUN_00215e00");
s32 music_transition(s32 target_track, s32 transition_track, s32 flags, s32 volume) {
    volatile s32 *location_entry;
    s64 stream_location;
    u8 *track_table;
    s32 track_table_offset;

    if (music_state.transition_handle != 0) {
        return 0;
    }
    track_table = (u8 *)&level_music_table;
    track_table_offset = 0x2AA8;
    location_entry = (s32 *)(track_table + track_table_offset) + transition_track;
    /* Keep both retail reads: the track location can change between them. */
    if (*location_entry == 0) {
        return 0;
    }
    *(u32 *)&music_state.transition_handle = 0xFFFFFFFF;
    music_state.transition_remaining_time = 48000;
    music_state.transition_poll_interval = 10;
    music_state.transition_track = target_track;
    music_state.transition_state = 1;
    music_state.transition_volume = volume;
    music_state.transition_crossfade_enabled = 1;
    music_state.transition_flags = flags;
    stream_location = *location_entry;
    snd_play_vag_stream_by_loc_ex_cb(stream_location, 0, 0, 0, volume, 0, 1, 0, 0x20, music_transition_start_callback, (u32)&music_state.transition_handle);
    return 1;
}
#endif /* NON_MATCHING */
