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
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"

/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002C8DE0), where it is exact; names translated to the US level program. */

extern int FUN_L00_0028dc90(int i, int a1, int v, int k);

void timed_effect_emitter_update(struct Moby *moby) __asm__("FUN_L10_002c7a20");

extern void rotation_matrix(void *, void *) __asm__("FUN_001fa050");

typedef struct TimedEffectEmitterVars {
    u8  pad_0[0x4];
    s32 active_duration;
    s32 idle_duration;
    s32 timer_C;
    s32 voice;
} TimedEffectEmitterVars;

/* Moby class 702 */
void timed_effect_emitter_update(struct Moby *moby) {
    enum {
        MOBY_STATE_WAIT_FOR_ACTIVATION = 0,
        MOBY_STATE_ACTIVE_WITH_SOUND = 1
    };
    TimedEffectEmitterVars *emitter_vars = (TimedEffectEmitterVars *)moby->pvars;
    float                   rotation_matrix[16];
    float                   position[4];
    float                   rotated_vector[4];
    float                   position_step[4];
    switch (moby->state) {
    case MOBY_STATE_WAIT_FOR_ACTIVATION:
        emitter_vars->voice = -1;
        if (fast_dec_timer(&emitter_vars->timer_C)) {
            emitter_vars->timer_C = emitter_vars->active_duration;
            moby->state = MOBY_STATE_ACTIVE_WITH_SOUND;
        }
        break;
    case MOBY_STATE_ACTIVE_WITH_SOUND:
        qcopy(position, &moby->pos);
        euler_rotation_matrix(rotation_matrix, &moby->rot.x);
        clear_u64_value(rotated_vector);
        rotated_vector[2] = *(float *)emitter_vars * 3.0f * frame_time;
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
        if (voice_is_playing(moby, emitter_vars->voice) == 0) {
            emitter_vars->voice = FUN_L00_0028dc90(0, 4, (int)moby, 0x519);
        }
        if (fast_dec_timer(&emitter_vars->timer_C)) {
            int k;
            emitter_vars->timer_C = emitter_vars->idle_duration;
            moby->state = MOBY_STATE_WAIT_FOR_ACTIVATION;
            k = emitter_vars->voice;
            if (k != -1) {
                unsigned char *voice_entry = (unsigned char *)&voice_pool + k * 0x70;
                if (*(unsigned char **)(voice_entry + 0x88) == moby && voice_entry[0x74] != 0) {
                    release_voice_slot(k);
                }
            }
            emitter_vars->voice = -1;
        }
        break;
    }
}
