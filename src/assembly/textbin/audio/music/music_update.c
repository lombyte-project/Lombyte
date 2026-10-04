#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/audio/music/music_update/FUN_00216290.s", FUN_00216290);
#else
#include "types.h"
#include "rnc/music_stream_state.h"

extern struct MusicStreamState D_001516D0;
extern u8 D_00151704[];
extern void func_0012E4C0(s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_00216B68();
extern void func_0012ECA0(s32);
extern s32 func_0012EE08(s32);
extern s32 func_001F96F8(s32);
extern s32 func_001F9740(void *);
extern void func_00215970(s32, s32, s32);
extern void func_00215B68(s32, s32, s32);
extern void func_00215C40(s32, s32, s32);
extern void func_00215D18(s32, s32, s32);
extern s32 func_00215E00(s32, s32, s32, s32);
extern void func_002160A8(void *);
extern s32 func_00216788(s32, s32, s32);

void music_update(void) __asm__("FUN_00216290");

void music_update(void) {
    s8 requested_track;
    s32 fade_volume;
    s32 handle;
    s16 pending_state;

    if (D_001516D0.updates_suspended != 0) {
        return;
    }
    if (!(D_001516D0.primary_fade_flags & 0x8000) && !(D_001516D0.primary_state & 0x8000)) {
        if (D_001516D0.primary_handle == 0 && (D_001516D0.primary_flags & 1) && D_001516D0.requested_track == -1) {
            func_00215C40(D_001516D0.primary_track, (s16)D_001516D0.primary_flags, D_001516D0.primary_volume);
        } else if (D_001516D0.primary_state != 9 && D_001516D0.primary_handle != 0) {
            if (D_001516D0.primary_handle != 0xFFFFFFFF && D_001516D0.primary_state == 8) {
                func_00215D18(D_001516D0.primary_track, D_001516D0.primary_flags, D_001516D0.primary_volume);
            }
        }
    }
    if (func_001F9740(&D_001516D0.retry_timer) != 0) {
        requested_track = D_001516D0.requested_track;
        if (requested_track != -1 && D_001516D0.crossfade_state == 0) {
            if (D_001516D0.primary_track != requested_track) {
                if (D_001516D0.requested_transition_track == -1 || func_00215E00(requested_track, D_001516D0.requested_transition_track, D_001516D0.primary_flags, D_001516D0.primary_volume) != 0) {
                    D_001516D0.retry_timer = func_001F96F8(7) * 60.0f;
                }
            } else {
                D_001516D0.requested_track = -1;
            }
        }
    }
    if (D_001516D0.queued_secondary_track >= 0) {
        if (D_001516D0.secondary_handle != 0) {
            if ((u16)D_001516D0.secondary_state - 6 >= 2U) {
                D_001516D0.secondary_state = 5;
            }
        } else {
            func_00215970(D_001516D0.queued_secondary_track, 0, 0x400);
            D_001516D0.queued_secondary_track = -1;
        }
    }
    if (D_001516D0.primary_state != 9 && D_001516D0.primary_handle != 0xFFFFFFFF && !(D_001516D0.primary_fade_flags & 0x8000) && !(D_001516D0.primary_state & 0x8000)
        && !(D_001516D0.transition_fade_flags & 0x8000) && !(D_001516D0.transition_state & 0x8000)) {
        switch (D_001516D0.crossfade_state) {
        case 2:
            /* Fade the primary stream against the transition's remaining time. */
            fade_volume = D_001516D0.primary_volume * (D_001516D0.crossfade_interval - (D_001516D0.crossfade_remaining_time - D_001516D0.transition_remaining_time)) / D_001516D0.crossfade_interval;
            if (fade_volume <= 0 || ((D_001516D0.transition_state != 4 || D_001516D0.transition_crossfade_enabled == 0) && D_001516D0.transition_handle == 0) || (handle = D_001516D0.primary_handle) == 0) {
                D_001516D0.crossfade_state = 3;
                D_001516D0.primary_state = 5;
            } else if (D_001516D0.primary_state != 9) {
                *(u32 *)&D_001516D0.primary_handle = 0xFFFFFFFF;
                func_0012E4C0(handle, 5, fade_volume, 0, 0, 0, (s32)func_00216B68, (s32)&D_001516D0.primary_handle);
            }
            break;
        case 3:
            if (D_001516D0.primary_state == 0 && (D_001516D0.transition_crossfade_enabled != 0 || D_001516D0.transition_handle == 0)) {
                func_00215B68(D_001516D0.requested_track, D_001516D0.primary_flags, D_001516D0.primary_volume);
                D_001516D0.requested_track = -1;
                D_001516D0.crossfade_state = 4;
            }
            break;
        case 4:
            if (D_001516D0.primary_state == 3) {
                if (D_001516D0.transition_remaining_time < D_001516D0.crossfade_interval || D_001516D0.transition_state != 4 || (D_001516D0.transition_crossfade_enabled == 0 && D_001516D0.transition_handle == 0)) {
                    func_0012ECA0(D_001516D0.primary_handle);
                    D_001516D0.primary_state = 8;
                    D_001516D0.crossfade_state = 5;
                }
            }
            break;
        case 5:
            if (D_001516D0.transition_state != 4 || D_001516D0.transition_crossfade_enabled == 0) {
                D_001516D0.crossfade_state = 0;
            }
            break;
        }
    }
    func_002160A8(&D_001516D0.primary_handle);
    func_002160A8(&D_001516D0.transition_handle);
    func_002160A8(&D_001516D0.secondary_handle);
    if (D_001516D0.stop_pending != 0) {
        if (func_0012EE08(1) == 0) {
            D_001516D0.pending_start_state = 0;
            D_001516D0.stop_pending = 0;
        }
    } else {
        pending_state = D_001516D0.pending_start_state;
        if (pending_state == 2) {
            D_001516D0.pending_start_state = 0;
            func_00216788(D_001516D0.queued_track, D_001516D0.queued_flags, D_001516D0.queued_volume);
            if (D_001516D0.pending_start_state == 0) {
                D_001516D0.pending_start_state = pending_state;
            }
        }
    }
}
#endif /* NON_MATCHING */
