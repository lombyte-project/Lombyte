/* Sound handler type 0: plays an emitter's sound while the camera is within its radius. */

#include "types.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/debug/debug_menu.h"

#define ENT_2ed950(i) ((E_2ed950 *)((u8 *)voice_pool_lui + (i) * 0x70))

typedef struct {
    s32 id;
    s32 w4;
    s32 w8;
    s32 wC;
    s32 snd;
} S_2ed950;

typedef struct {
    u8    p[0x74];
    u8    b74;
    u8    p2[0x8C - 0x75];
    void *o;
} E_2ed950;

typedef struct {
    u8 p[0x18];
    u8 b18;
    u8 p2[7];
} T_2ed950;

s32 play_voice(s32, s32, void *, s32) __asm__("FUN_L00_0028ddd8");
s32 FUN_001f96f8_2ed950(s32) __asm__("FUN_001f96f8");

extern E_2ed950  voice_pool_lui[] __asm__("D_0013E550") NOT_SDA;
extern T_2ed950 *D_L00_0015F5F4_2ed950 __asm__("D_L00_0015F5F4");

T_2ed950 *D_L01_0015F5F4 = 0;
T_2ed950 *D_L02_0015F5F4 = 0;
T_2ed950 *D_L03_0015F5F4 = 0;
T_2ed950 *D_L04_0015F5F4 = 0;
T_2ed950 *D_L05_0015F5F4 = 0;
T_2ed950 *D_L08_0015F5F4 = 0;
T_2ed950 *D_L09_0015F5F4 = 0;
T_2ed950 *D_L10_0015F5F4 = 0;
T_2ed950 *D_L11_0015F5F4 = 0;
T_2ed950 *D_L12_0015F5F4 = 0;
T_2ed950 *D_L13_0015F5F4 = 0;
T_2ed950 *D_L14_0015F5F4 = 0;
T_2ed950 *D_L15_0015F5F4 = 0;
T_2ed950 *D_L16_0015F5F4 = 0;
T_2ed950 *D_L17_0015F5F4 = 0;

void update_sound_emitter_playback_by_distance(u8 *sound_emitter) __asm__("FUN_L00_002ed950");

void update_sound_emitter_playback_by_distance(u8 *sound_emitter) {
    S_2ed950 *playback_state = *(S_2ed950 **)(sound_emitter + 8);
    f32       reference_distance, playback_radius;
    if (playback_state->id < 0)
        return;
    reference_distance = fast_vec_dist(g_debug_camera.position, sound_emitter + 0x40);
    playback_radius = *(f32 *)(sound_emitter + 0xC);
    if (reference_distance < playback_radius) {
        E_2ed950 *e = ENT_2ed950(playback_state->snd);
        s32       fl = 0;
        s32       four = 4;
        if (D_L00_0015F5F4_2ed950[playback_state->id].b18)
            fl = four;
        if (e->o == sound_emitter && e->b74)
            return;
        if (!fast_dec_timer(&playback_state->wC))
            return;
        playback_state->snd = play_voice(playback_state->id, fl, sound_emitter, 0x400);
        if (playback_state->w8 > 0) {
            playback_state->wC =
                FUN_001f96f8_2ed950(playback_state->w4 +
                                    random_integer_below(playback_state->w8 - playback_state->w4)) *
                60.0f;
        }
    } else if (playback_radius + 1.0f < reference_distance) {
        s32 voice_slot_index = playback_state->snd;
        if (voice_slot_index != -1) {
            E_2ed950 *e = ENT_2ed950(voice_slot_index);
            if (e->o == sound_emitter && e->b74)
                release_voice_slot(voice_slot_index);
        }
        playback_state->snd = -1;
    }
}
