#ifndef LOMBYTE_RNC_AUDIO_MUSIC_MUSIC_STREAM_STATE_H
#define LOMBYTE_RNC_AUDIO_MUSIC_MUSIC_STREAM_STATE_H

#include "types.h"

/* Sound/music stream state at 0x001516D0, shared by the snd/music units.
   Field widths come from the retail accesses of those units (majority sign
   where lh/lhu both occur).  Three identical 0x1C-byte channel records start
   at 0x34, 0x50 and 0x6C: s32 +0, s16 +4..+10, s32 +14, s32 +18.  The fields
   remain flat to preserve the existing access expressions and scheduling. */
struct MusicStreamState {
    s32 unk0;
    u8 pad_4[0x4];
    s16 pending_start_state;
    u8 stop_pending;
    u8 updates_suspended;
    s32 queued_flags;
    s32 queued_volume;
    s32 queued_track;
    u8 pad_18[0x4];
    s32 queued_secondary_track;
    s16 crossfade_state;
    s8 requested_track;
    s8 requested_transition_track;
    s32 crossfade_remaining_time;
    s32 crossfade_interval;
    s32 retry_timer;
    u8 unk30;
    u8 unk31;
    u8 unk32;
    u8 unk33;
    /* channel record 0 */
    s32 primary_handle;
    s16 primary_track;
    s16 primary_volume;
    s16 primary_flags;
    s16 primary_state;
    s16 primary_fade_flags;
    u8 pad_42[0x2];
    s16 primary_crossfade_enabled;
    u8 pad_46[0x2];
    s32 primary_poll_interval;
    s32 primary_remaining_time;
    /* channel record 1 */
    s32 secondary_handle;
    s16 secondary_track;
    s16 secondary_volume;
    s16 secondary_flags;
    s16 secondary_state;
    s16 secondary_fade_flags;
    u8 pad_5E[0x2];
    s16 secondary_crossfade_enabled;
    u8 pad_62[0x2];
    s32 secondary_poll_interval;
    s32 secondary_remaining_time;
    /* channel record 2 */
    s32 transition_handle;
    s16 transition_track;
    s16 transition_volume;
    s16 transition_flags;
    s16 transition_state;
    s16 transition_fade_flags;
    u8 pad_7A[0x2];
    s16 transition_crossfade_enabled;
    u8 pad_7E[0x2];
    s32 transition_poll_interval;
    s32 transition_remaining_time;
};

#endif /* LOMBYTE_RNC_AUDIO_MUSIC_MUSIC_STREAM_STATE_H */
