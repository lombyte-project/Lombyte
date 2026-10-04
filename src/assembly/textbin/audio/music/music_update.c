#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/audio/music/music_update/FUN_00216290.s", FUN_00216290);
#else
#include "types.h"
#include "rnc/music_stream_state.h"

extern struct MusicStreamState music_state __asm__("D_001516D0");
extern u8 D_00151704[];
extern void snd_set_sound_params_cb(s32, s32, s32, s32, s32, s32, s32, s32) __asm__("func_0012E4C0");
extern s32 set_sound_handle_id() __asm__("func_00216B68");
extern void snd_continue_vag_stream(s32) __asm__("func_0012ECA0");
extern s32 snd_stream_safe_cd_sync(s32) __asm__("func_0012EE08");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 func_001F9740(void *);
extern void music_start_track_by_id(s32, s32, s32) __asm__("func_00215970");
extern void music_preseek_track(s32, s32, s32) __asm__("func_00215B68");
extern void music_start_track(s32, s32, s32) __asm__("func_00215C40");
extern void music_start_track_body(s32, s32, s32) __asm__("func_00215D18");
extern s32 music_transition(s32, s32, s32, s32) __asm__("func_00215E00");
extern void music_update_stream(void *) __asm__("func_002160A8");
extern s32 start_audio_stream_read(s32, s32, s32) __asm__("func_00216788");

void music_update(void) __asm__("FUN_00216290");

void music_update(void) {
    s8 requested_track;
    s32 fade_volume;
    s32 handle;
    s16 pending_state;

    if (music_state.updates_suspended != 0) {
        return;
    }
    if (!(music_state.primary_fade_flags & 0x8000) && !(music_state.primary_state & 0x8000)) {
        if (music_state.primary_handle == 0 && (music_state.primary_flags & 1) && music_state.requested_track == -1) {
            music_start_track(music_state.primary_track, (s16)music_state.primary_flags, music_state.primary_volume);
        } else if (music_state.primary_state != 9 && music_state.primary_handle != 0) {
            if (music_state.primary_handle != 0xFFFFFFFF && music_state.primary_state == 8) {
                music_start_track_body(music_state.primary_track, music_state.primary_flags, music_state.primary_volume);
            }
        }
    }
    if (func_001F9740(&music_state.retry_timer) != 0) {
        requested_track = music_state.requested_track;
        if (requested_track != -1 && music_state.crossfade_state == 0) {
            if (music_state.primary_track != requested_track) {
                if (music_state.requested_transition_track == -1 || music_transition(requested_track, music_state.requested_transition_track, music_state.primary_flags, music_state.primary_volume) != 0) {
                    music_state.retry_timer = scale_game_frames(7) * 60.0f;
                }
            } else {
                music_state.requested_track = -1;
            }
        }
    }
    if (music_state.queued_secondary_track >= 0) {
        if (music_state.secondary_handle != 0) {
            if ((u16)music_state.secondary_state - 6 >= 2U) {
                music_state.secondary_state = 5;
            }
        } else {
            music_start_track_by_id(music_state.queued_secondary_track, 0, 0x400);
            music_state.queued_secondary_track = -1;
        }
    }
    if (music_state.primary_state != 9 && music_state.primary_handle != 0xFFFFFFFF && !(music_state.primary_fade_flags & 0x8000) && !(music_state.primary_state & 0x8000)
        && !(music_state.transition_fade_flags & 0x8000) && !(music_state.transition_state & 0x8000)) {
        switch (music_state.crossfade_state) {
        case 2:
            /* Fade the primary stream against the transition's remaining time. */
            fade_volume = music_state.primary_volume * (music_state.crossfade_interval - (music_state.crossfade_remaining_time - music_state.transition_remaining_time)) / music_state.crossfade_interval;
            if (fade_volume <= 0 || ((music_state.transition_state != 4 || music_state.transition_crossfade_enabled == 0) && music_state.transition_handle == 0) || (handle = music_state.primary_handle) == 0) {
                music_state.crossfade_state = 3;
                music_state.primary_state = 5;
            } else if (music_state.primary_state != 9) {
                *(u32 *)&music_state.primary_handle = 0xFFFFFFFF;
                snd_set_sound_params_cb(handle, 5, fade_volume, 0, 0, 0, (s32)set_sound_handle_id, (s32)&music_state.primary_handle);
            }
            break;
        case 3:
            if (music_state.primary_state == 0 && (music_state.transition_crossfade_enabled != 0 || music_state.transition_handle == 0)) {
                music_preseek_track(music_state.requested_track, music_state.primary_flags, music_state.primary_volume);
                music_state.requested_track = -1;
                music_state.crossfade_state = 4;
            }
            break;
        case 4:
            if (music_state.primary_state == 3) {
                if (music_state.transition_remaining_time < music_state.crossfade_interval || music_state.transition_state != 4 || (music_state.transition_crossfade_enabled == 0 && music_state.transition_handle == 0)) {
                    snd_continue_vag_stream(music_state.primary_handle);
                    music_state.primary_state = 8;
                    music_state.crossfade_state = 5;
                }
            }
            break;
        case 5:
            if (music_state.transition_state != 4 || music_state.transition_crossfade_enabled == 0) {
                music_state.crossfade_state = 0;
            }
            break;
        }
    }
    music_update_stream(&music_state.primary_handle);
    music_update_stream(&music_state.transition_handle);
    music_update_stream(&music_state.secondary_handle);
    if (music_state.stop_pending != 0) {
        if (snd_stream_safe_cd_sync(1) == 0) {
            music_state.pending_start_state = 0;
            music_state.stop_pending = 0;
        }
    } else {
        pending_state = music_state.pending_start_state;
        if (pending_state == 2) {
            music_state.pending_start_state = 0;
            start_audio_stream_read(music_state.queued_track, music_state.queued_flags, music_state.queued_volume);
            if (music_state.pending_start_state == 0) {
                music_state.pending_start_state = pending_state;
            }
        }
    }
}
#endif /* NON_MATCHING */
