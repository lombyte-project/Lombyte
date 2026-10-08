#include "types.h"
#include "asm.h"

#include "types.h"
#include "rnc/audio/music/music_stream_state.h"

extern u8 D_00151704[];
extern void snd_set_sound_params_cb(s32, s32, s32, s32, s32, s32,
                                    void (*)(u32, s64), s64) __asm__("func_0012E4C0");
extern void set_sound_handle_id(u32, s64) __asm__("func_00216B68");
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
extern s32 start_audio_stream_read(s32, s32, s32) __asm__("FUN_00216788");

void music_update(void) __asm__("FUN_00216290");

void music_update(void) {
    s8 requested_track;
    s32 fade_volume;
    s32 handle;
    s16 pending_state;

    if (music_stream_state.updates_suspended != 0) {
        return;
    }
    if (!(music_stream_state.primary.fade_flags & 0x8000) && !(music_stream_state.primary.state & 0x8000)) {
        if (music_stream_state.primary.handle == 0 && (music_stream_state.primary.flags & 1) &&
            music_stream_state.requested_track == -1) {
            music_start_track(music_stream_state.primary.track, (s16)music_stream_state.primary.flags,
                              music_stream_state.primary.volume);
        } else if (music_stream_state.primary.state != 9 && music_stream_state.primary.handle != 0) {
            if (music_stream_state.primary.handle != 0xFFFFFFFF && music_stream_state.primary.state == 8) {
                music_start_track_body(music_stream_state.primary.track, music_stream_state.primary.flags,
                                       music_stream_state.primary.volume);
            }
        }
    }
    if (func_001F9740(&music_stream_state.retry_timer) != 0) {
        requested_track = music_stream_state.requested_track;
        if (requested_track != -1 && music_stream_state.crossfade_state == 0) {
            if (music_stream_state.primary.track != requested_track) {
                if (music_stream_state.requested_transition_track == -1 ||
                    music_transition(requested_track, music_stream_state.requested_transition_track,
                                     music_stream_state.primary.flags, music_stream_state.primary.volume) != 0) {
                    music_stream_state.retry_timer = scale_game_frames(7) * 60.0f;
                }
            } else {
                music_stream_state.requested_track = -1;
            }
        }
    }
    if (music_stream_state.queued_secondary_track >= 0) {
        if (music_stream_state.secondary.handle != 0) {
            if ((u16)music_stream_state.secondary.state - 6 >= 2U) {
                music_stream_state.secondary.state = 5;
            }
        } else {
            music_start_track_by_id(music_stream_state.queued_secondary_track, 0, 0x400);
            music_stream_state.queued_secondary_track = -1;
        }
    }
    if (music_stream_state.primary.state != 9 && music_stream_state.primary.handle != 0xFFFFFFFF &&
        !(music_stream_state.primary.fade_flags & 0x8000) && !(music_stream_state.primary.state & 0x8000) &&
        !(music_stream_state.transition.fade_flags & 0x8000) && !(music_stream_state.transition.state & 0x8000)) {
        switch (music_stream_state.crossfade_state) {
        case 2:
            /* Fade the primary stream against the transition's remaining time. */
            fade_volume =
                music_stream_state.primary.volume *
                (music_stream_state.crossfade_interval -
                 (music_stream_state.crossfade_remaining_time - music_stream_state.transition.remaining_time)) /
                music_stream_state.crossfade_interval;
            if (fade_volume <= 0 ||
                ((music_stream_state.transition.state != 4 ||
                  music_stream_state.transition.crossfade_enabled == 0) &&
                 music_stream_state.transition.handle == 0) ||
                (handle = music_stream_state.primary.handle) == 0) {
                music_stream_state.crossfade_state = 3;
                music_stream_state.primary.state = 5;
            } else if (music_stream_state.primary.state != 9) {
                /* The callback receives the old handle; mark its slot pending before submission. */
                *(u32 *)&music_stream_state.primary.handle = 0xFFFFFFFF;
                snd_set_sound_params_cb(handle, 5, fade_volume, 0, 0, 0, set_sound_handle_id,
                                        (s64)&music_stream_state.primary.handle);
            }
            break;
        case 3:
            if (music_stream_state.primary.state == 0 && (music_stream_state.transition.crossfade_enabled != 0 ||
                                                   music_stream_state.transition.handle == 0)) {
                music_preseek_track(music_stream_state.requested_track, music_stream_state.primary.flags,
                                    music_stream_state.primary.volume);
                music_stream_state.requested_track = -1;
                music_stream_state.crossfade_state = 4;
            }
            break;
        case 4:
            if (music_stream_state.primary.state == 3) {
                if (music_stream_state.transition.remaining_time < music_stream_state.crossfade_interval ||
                    music_stream_state.transition.state != 4 ||
                    (music_stream_state.transition.crossfade_enabled == 0 &&
                     music_stream_state.transition.handle == 0)) {
                    snd_continue_vag_stream(music_stream_state.primary.handle);
                    music_stream_state.primary.state = 8;
                    music_stream_state.crossfade_state = 5;
                }
            }
            break;
        case 5:
            if (music_stream_state.transition.state != 4 ||
                music_stream_state.transition.crossfade_enabled == 0) {
                music_stream_state.crossfade_state = 0;
            }
            break;
        }
    }
    music_update_stream(&music_stream_state.primary.handle);
    music_update_stream(&music_stream_state.transition.handle);
    music_update_stream(&music_stream_state.secondary.handle);
    if (music_stream_state.break_requested != 0) {
        if (snd_stream_safe_cd_sync(1) == 0) {
            music_stream_state.read_state = 0;
            music_stream_state.break_requested = 0;
        }
    } else {
        pending_state = music_stream_state.read_state;
        if (pending_state == 2) {
            music_stream_state.read_state = 0;
            start_audio_stream_read(music_stream_state.read_dst, music_stream_state.read_sector,
                                    music_stream_state.read_sector_count);
            if (music_stream_state.read_state == 0) {
                music_stream_state.read_state = pending_state;
            }
        }
    }
}

extern __typeof__(music_update) func_00216290 __attribute__((alias("FUN_00216290")));
