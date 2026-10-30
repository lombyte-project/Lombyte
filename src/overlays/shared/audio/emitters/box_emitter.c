/* Sound emitter type 1: plays a voice at the point of the emitter box nearest the listener. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/quad.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/sound_state.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/audio/voices/play_voice.h"
#include "rnc/overlay/shared/vendor/vendor_volume.h"

extern char D_L01_00167240[];

extern char *D_L00_0015F5F4;
extern char  D_0013E550[];
extern int   calculate_voice_distance_volume(float, float, float, void *) __asm__("FUN_0022c6f8");
extern int   random_integer_below(int) __asm__("FUN_00213260");

void update_box_emitter(char *moby) __asm__("FUN_L01_00319928");

void update_box_emitter(char *moby) {
    OvlVec4 delta, radius, box_size, listener;
    int    *sound_data = *(int **)(moby + 8);
    int     volume;
    int     flags;
    float   box_length, listener_length;

    radius.f[2] = radius.f[1] = radius.f[0] = MOBY(moby)->bsphere.w;
    fast_vec_sub(&delta, D_L01_00167240, moby + 0x40);
    delta.f[3] = 0.0f;
    transform_vector_by_basis(&box_size, &delta, moby + 0x50);
    transform_vector_by_basis(&listener, &radius, moby + 0x50);
    listener_length = fast_vec_length(&listener);
    box_length = fast_vec_length(&box_size);
    if (box_length < listener_length) {
        flags = 0x10;
        if (absolute_float(box_size.f[0]) <= 1.0f && absolute_float(box_size.f[1]) <= 1.0f &&
            absolute_float(box_size.f[2]) <= 1.0f) {
            volume = *(int *)(D_L00_0015F5F4 + sound_data[0] * 0x20 + 0xc);
        } else {
            if (box_size.f[0] > 1.0f)
                box_size.f[0] = 1.0f;
            else if (box_size.f[0] < -1.0f)
                box_size.f[0] = -1.0f;
            if (box_size.f[1] > 1.0f)
                box_size.f[1] = 1.0f;
            else if (box_size.f[1] < -1.0f)
                box_size.f[1] = -1.0f;
            if (box_size.f[2] > 1.0f)
                box_size.f[2] = 1.0f;
            else if (box_size.f[2] < -1.0f)
                box_size.f[2] = -1.0f;
            volume = calculate_voice_distance_volume(box_length,
                                                     fast_vec_length(&box_size),
                                                     listener_length,
                                                     D_L00_0015F5F4 + sound_data[0] * 0x20);
        }
        if (*(u8 *)(D_L00_0015F5F4 + sound_data[0] * 0x20 + 0x18))
            flags = 0x14;
        {
            char *slot = D_0013E550 + sound_data[4] * 0x70;
            if (*(char **)(slot + 0x8c) != moby || *(u8 *)(slot + 0x74) == 0) {
                if (fast_dec_timer(&sound_data[3])) {
                    sound_data[4] = play_voice(sound_data[0], flags, moby, volume);
                    if (sound_data[2] > 0) {
                        sound_data[3] =
                            (int)((float)scale_game_frames(
                                      sound_data[1] +
                                      random_integer_below(sound_data[2] - sound_data[1])) *
                                  60.0f);
                    }
                } else {
                    sound_data[4] = -1;
                }
            }
        }
        if (sound_data[4] != -1) {
            vendor_volume(sound_data[4], volume);
            transform_vector_by_basis(&delta, &box_size, moby + 0x10);
            fast_vec_add(&delta, &delta, moby + 0x40);
            qcopy(voice_positions + sound_data[4] * 0x70, &delta);
        }
    } else {
        if (sound_data[4] != -1) {
            char *slot = D_0013E550 + sound_data[4] * 0x70;
            if (*(char **)(slot + 0x8c) == moby && *(u8 *)(slot + 0x74) != 0)
                release_voice_slot(sound_data[4]);
        }
        sound_data[4] = -1;
    }
}
