#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/streaming/disc_table.h"

extern void music_transition_start_callback() __asm__("FUN_00216a80");
extern s32  snd_play_vag_stream_by_loc_ex_cb(
     s32, s32, s32, s32, s16, s32, s32, s32, s32, void (*)(), u64) __asm__("func_0012EC08");

s32 music_transition(s32 target_track,
                     s32 transition_track,
                     s32 flags,
                     s32 volume) __asm__("FUN_00215e00");

s32 music_transition(s32 target_track, s32 transition_track, s32 flags, s32 volume) {
    s32 *transition_track_location_entry;
    u8  *disc_table_base;
    s32  track_location_table_offset;
    s32  stream_request_result;
    s64  stream_location;

    if (music_stream_state.transition.handle == 0) {
        disc_table_base = (u8 *)&disc_table;
        track_location_table_offset = 0x2AA8;
        transition_track_location_entry =
            (s32 *)(disc_table_base + track_location_table_offset) + transition_track;
        if (*transition_track_location_entry != 0) {
            *(u32 *)&music_stream_state.transition.handle = 0xFFFFFFFF;
            music_stream_state.transition.poll_interval = 10;
            music_stream_state.transition.remaining_time = 48000;
            music_stream_state.transition.track = target_track;
            music_stream_state.transition.state = 1;
            music_stream_state.transition.volume = volume;
            music_stream_state.transition.crossfade_enabled = 1;
            music_stream_state.transition.flags = flags;
            /* Fakematch: the never-negative test gives `return 1` its own block
               until combine folds it away, as in retail's block layout. */
            stream_location = *transition_track_location_entry;
            stream_request_result =
                (u16)snd_play_vag_stream_by_loc_ex_cb(stream_location,
                                                      0,
                                                      0,
                                                      0,
                                                      volume,
                                                      0,
                                                      1,
                                                      0,
                                                      0x20,
                                                      music_transition_start_callback,
                                                      (u32)&music_stream_state.transition.handle);
            if (stream_request_result < 0) {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

extern __typeof__(music_transition) func_00215E00 __attribute__((alias("FUN_00215e00")));
