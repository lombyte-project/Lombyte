#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/audio/music/music_transition/FUN_00215e00.s", FUN_00215e00);
#else
#include "types.h"
#include "rnc/music_stream_state.h"
struct MusicTable { u8 pad0[0x2AA8]; s32 tracks[1]; };
extern struct MusicTable D_00137B80;
extern struct MusicStreamState D_001516D0;
extern void FUN_00216a80();
extern void func_0012EC08(s32, s32, s32, s32, s16, s32, s32, s32, s32, void (*)(), u64);
s32 music_transition(s32 target_track, s32 transition_track, s32 flags, s32 volume) __asm__("FUN_00215e00");
s32 music_transition(s32 target_track, s32 transition_track, s32 flags, s32 volume) {
    volatile s32 *location_entry;
    s64 stream_location;
    u8 *track_table;
    s32 track_table_offset;

    if (D_001516D0.transition_handle != 0) {
        return 0;
    }
    track_table = (u8 *)&D_00137B80;
    track_table_offset = 0x2AA8;
    location_entry = (s32 *)(track_table + track_table_offset) + transition_track;
    /* Keep both retail reads: the track location can change between them. */
    if (*location_entry == 0) {
        return 0;
    }
    *(u32 *)&D_001516D0.transition_handle = 0xFFFFFFFF;
    D_001516D0.transition_remaining_time = 48000;
    D_001516D0.transition_poll_interval = 10;
    D_001516D0.transition_track = target_track;
    D_001516D0.transition_state = 1;
    D_001516D0.transition_volume = volume;
    D_001516D0.transition_crossfade_enabled = 1;
    D_001516D0.transition_flags = flags;
    stream_location = *location_entry;
    func_0012EC08(stream_location, 0, 0, 0, volume, 0, 1, 0, 0x20, FUN_00216a80, (u32)&D_001516D0.transition_handle);
    return 1;
}
#endif /* NON_MATCHING */
