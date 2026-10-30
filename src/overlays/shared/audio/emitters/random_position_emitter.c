/* Sound handler type 2: plays an emitter's sound at random points of its box while the camera is inside it. */

#include "types.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/math/fast_math.h"
#include "rnc/audio/voice_pool.h"
#include "qcopy.h"

typedef struct {
    s32 snd;
    s32 lo;
    s32 hi;
    s32 timer;
    s32 h;
} S_2edad8;

typedef struct {
    f32 f[4];
} V_2edad8;

s32         play_voice(s32, s32, void *, s32) __asm__("FUN_L00_0028ddd8");
extern void sub_2edad8(void *, void *, void *) __asm__("FUN_001f9a28");

extern u8 g_debug_camera_lui[] __asm__("D_L00_00166DC0") NOT_SDA;

void update_sound_emitter_playback_at_random_position(u8 *sound_emitter) __asm__("FUN_L00_002edad8");

void update_sound_emitter_playback_at_random_position(u8 *sound_emitter) {
    V_2edad8  reference_position_delta;
    V_2edad8  uniform_range_vector;
    V_2edad8  transformed_reference_position_delta;
    V_2edad8  b;
    V_2edad8  r;
    S_2edad8 *playback_state;
    f32       f;
    u8       *sound_slot_entry;
    playback_state = *(S_2edad8 **)(sound_emitter + 8);
    {
        f32 g = *(f32 *)(sound_emitter + 0xC);
        uniform_range_vector.f[0] = g;
        uniform_range_vector.f[1] = g;
        uniform_range_vector.f[2] = g;
    }
    sub_2edad8(&reference_position_delta, g_debug_camera_lui, sound_emitter + 0x40);
    reference_position_delta.f[3] = 0;
    transform_vector_by_basis(
        &transformed_reference_position_delta, &reference_position_delta, sound_emitter + 0x50);
    transform_vector_by_basis(&b, &uniform_range_vector, sound_emitter + 0x50);
    f = absolute_float(transformed_reference_position_delta.f[0]);
    if (!(f <= absolute_float(b.f[0]) + 1.0f))
        return;
    f = absolute_float(transformed_reference_position_delta.f[1]);
    if (!(f <= absolute_float(b.f[1]) + 1.0f))
        return;
    f = absolute_float(transformed_reference_position_delta.f[2]);
    if (!(f <= absolute_float(b.f[2]) + 1.0f))
        return;
    sound_slot_entry = (u8 *)&voice_pool + playback_state->h * 0x70;
    if (*(u8 **)(sound_slot_entry + 0x8C) == sound_emitter && sound_slot_entry[0x74] != 0)
        return;
    if (fast_dec_timer(&playback_state->timer) != 0) {
        playback_state->h = play_voice(playback_state->snd, 0, sound_emitter, 0x400);
        if (playback_state->h != -1) {
            r.f[0] = random_float_between(-1.0f, 1.0f);
            r.f[1] = random_float_between(-1.0f, 1.0f);
            r.f[2] = random_float_between(-1.0f, 1.0f);
            transform_vector_by_basis(&r, &r, sound_emitter + 0x10);
            fast_vec_add(&r, &r, sound_emitter + 0x40);
            qcopy((u8 *)&voice_pool + 0x90 + playback_state->h * 0x70, &r);
        }
        if (playback_state->hi > 0) {
            playback_state->timer =
                (f32)scale_game_frames(
                    playback_state->lo +
                    random_integer_below(playback_state->hi - playback_state->lo)) *
                60.0f;
        }
    } else {
        playback_state->h = -1;
    }
}
