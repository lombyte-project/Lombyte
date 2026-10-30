/* Level 10 moby class timed_effect_emitter. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/mobys/moby_spawn.h"

#include "qcopy.h"
#include "rnc/audio/voice_pool.h"

/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002C8DE0), where it is exact; names translated to the US level program. */

extern int   FUN_L00_0028dc90(int i, int a1, int v, int k);
extern void *spawn_splitting_effect_particle_with_collision_response_and_fade(
    char *a, char *b, unsigned char c, int d) __asm__("FUN_L00_0026fd28");

void update_timed_effect_emitter(struct Moby *moby) __asm__("FUN_L10_002c7a20");

extern void rotation_matrix(void *, void *) __asm__("FUN_001fa050");

void update_timed_effect_emitter(struct Moby *moby) {
    enum {
        MOBY_STATE_WAIT_FOR_ACTIVATION = 0,
        MOBY_STATE_ACTIVE_WITH_SOUND = 1
    };
    char *data = (char *)moby->pvars;
    float rotation_matrix[16];
    float position[4];
    float rotated_vector[4];
    float position_step[4];
    switch (moby->state) {
    case MOBY_STATE_WAIT_FOR_ACTIVATION:
        *(int *)(data + 0x10) = -1;
        if (fast_dec_timer((int *)(data + 0xC))) {
            *(int *)(data + 0xC) = *(int *)(data + 4);
            moby->state = 1;
        }
        break;
    case MOBY_STATE_ACTIVE_WITH_SOUND:
        qcopy(position, &moby->pos);
        euler_rotation_matrix(rotation_matrix, &moby->rot.x);
        clear_u64_value(rotated_vector);
        rotated_vector[2] = *(float *)data * 3.0f * frame_time;
        transform_vector(rotated_vector, rotated_vector, rotation_matrix);
        fast_vec_scale(position_step, rotated_vector, 0.33f);
        spawn_splitting_effect_particle_with_collision_response_and_fade(
            (char *)position, (char *)rotated_vector, 0x40, (int)moby);
        fast_vec_add(position, position, position_step);
        spawn_splitting_effect_particle_with_collision_response_and_fade(
            (char *)position, (char *)rotated_vector, 0x40, (int)moby);
        fast_vec_add(position, position, position_step);
        spawn_splitting_effect_particle_with_collision_response_and_fade(
            (char *)position, (char *)rotated_vector, 0x40, (int)moby);
        if (voice_is_playing(moby, *(int *)(data + 0x10)) == 0) {
            *(int *)(data + 0x10) = FUN_L00_0028dc90(0, 4, (int)moby, 0x519);
        }
        if (fast_dec_timer((int *)(data + 0xC))) {
            int k;
            *(int *)(data + 0xC) = *(int *)(data + 8);
            moby->state = 0;
            k = *(int *)(data + 0x10);
            if (k != -1) {
                unsigned char *voice_entry = (unsigned char *)&voice_pool + k * 0x70;
                if (*(unsigned char **)(voice_entry + 0x88) == moby && voice_entry[0x74] != 0) {
                    release_voice_slot(k);
                }
            }
            *(int *)(data + 0x10) = -1;
        }
        break;
    }
}
