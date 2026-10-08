#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/audio/music/music_transition/FUN_00215e00.s",
            FUN_00215e00);
#else
#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/storage/disc_table.h"
extern void music_transition_start_callback() __asm__("FUN_00216a80");
extern void snd_play_vag_stream_by_loc_ex_cb(s32, s32, s32, s32, s16, s32, s32, s32, s32,
                                             void (*)(), u64) __asm__("func_0012EC08");
s32 music_transition(s32 target_track, s32 transition_track, s32 flags,
                     s32 volume) __asm__("FUN_00215e00");
s32 music_transition(s32 target_track, s32 transition_track, s32 flags, s32 volume) {
    volatile s32 *location_entry;
    s64 stream_location;
    u64 callback_context;
    void (*callback)();
    u8 *track_table;
    s32 track_table_offset;
    s32 poll_interval;
    s32 sample_rate;

    if (music_stream_state.transition.handle != 0) {
        return 0;
    }
    track_table = (u8 *)&disc_table;
    track_table_offset = 0x2AA8;
    location_entry = (s32 *)(track_table + track_table_offset) + transition_track;
    /* Keep both retail reads: the track location can change between them. */
    if (*location_entry == 0) {
        return 0;
    }
    *(u32 *)&music_stream_state.transition.handle = 0xFFFFFFFF;
    sample_rate = 48000;
    poll_interval = 10;
    music_stream_state.transition.poll_interval = poll_interval;
    music_stream_state.transition.remaining_time = sample_rate;
    music_stream_state.transition.track = target_track;
    music_stream_state.transition.state = 1;
    music_stream_state.transition.volume = volume;
    music_stream_state.transition.crossfade_enabled = 1;
    music_stream_state.transition.flags = flags;
    stream_location = *location_entry;
    callback_context = (u32)&music_stream_state.transition.handle;
    callback = music_transition_start_callback;
    snd_play_vag_stream_by_loc_ex_cb(stream_location, 0, 0, 0, volume, 0, 1, 0, 0x20, callback,
                                     callback_context);
    return 1;
}
#endif /* NON_MATCHING */
