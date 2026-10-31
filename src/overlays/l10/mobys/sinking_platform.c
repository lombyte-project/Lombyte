/* Level 10 moby class sinking_platform. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_00296DC0), where it is exact; names translated to the US level program. */

extern int  is_hero_on_moby(void *) __asm__("FUN_L01_00277fb8");
extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void update_moby_rotation_oscillation(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");

void sinking_platform_update(struct Moby *moby) __asm__("FUN_L10_00295c20");

/* Moby class 1117 */
void sinking_platform_update(struct Moby *moby) {
    enum {
        PLATFORM_STATE_INITIALIZE = 0,
        PLATFORM_STATE_BOBBING = 1,
        PLATFORM_STATE_SINKING_WITH_HERO = 2,
        PLATFORM_STATE_RETURN_TO_ORIGINAL_HEIGHT = 3
    };
    char *platform_vars = (char *)moby->pvars;
    float previous_position[4];
    float position_delta[4];
    *(OvlQuad *)previous_position = *(OvlQuad *)&moby->pos;
    if (is_hero_on_moby(moby))
        moby->state = 2;
    fast_dec_timer((int *)(platform_vars + 0xBC));
    switch (moby->state) {
    case PLATFORM_STATE_INITIALIZE:
        platform_vars[0x28] = 4;
        *(short *)(platform_vars + 0x3E) = 5;
        *(int *)(platform_vars + 0x20) = 0;
        *(short *)(platform_vars + 0x24) = 0;
        *(float *)(platform_vars + 0xA0) = moby->pos.z;
        *(float *)(platform_vars + 0xA8) = random_angle_radians();
        *(float *)(platform_vars + 0xB4) = random_angle_radians();
        {
            float initial_second_rotation_phase = random_angle_radians();
            float bobbing_phase_jitter_limit = frame_time * 0.13962634f;
            *(float *)(platform_vars + 0xB8) = initial_second_rotation_phase;
            *(float *)(platform_vars + 0xB0) =
                frame_time * 1.5707964f + randf_sym(0.0f, bobbing_phase_jitter_limit);
        }
        moby->state = 1;
    case PLATFORM_STATE_BOBBING:
        *(int *)(platform_vars + 0xA4) = 0;
        update_moby_vertical_oscillation(0.1f,
                                         *(float *)(platform_vars + 0xB0),
                                         moby,
                                         (float *)(platform_vars + 0xA8),
                                         (float *)(platform_vars + 0xAC));
        update_moby_rotation_oscillation(moby,
                                         platform_vars + 0xB4,
                                         platform_vars + 0xB8,
                                         0.047123890f,
                                         frame_time * 0.34906584f,
                                         frame_time * 0.5235988f);
        break;
    case PLATFORM_STATE_SINKING_WITH_HERO: {
        float a = *(float *)(platform_vars + 0xA4) - (frame_time_sq + frame_time_sq);
        float sinking_step_lower_bound = -(frame_time + frame_time);
        *(float *)(platform_vars + 0xA4) = a;
        if (a < sinking_step_lower_bound)
            *(float *)(platform_vars + 0xA4) = sinking_step_lower_bound;
        moby->pos.z += *(float *)(platform_vars + 0xA4);
        if (is_hero_on_moby(moby) == 0)
            moby->state = 3;
        if (*(int *)(platform_vars + 0xBC) <= 0) {
            allocate_voice_for_target_entry(0, 0, (int)moby);
            *(int *)(platform_vars + 0xBC) = random_int_between(0x28, 0x3C);
        }
        break;
    }
    case PLATFORM_STATE_RETURN_TO_ORIGINAL_HEIGHT: {
        float vz = *(float *)(platform_vars + 0xA4);
        float return_acceleration = frame_time_sq;
        float remaining_return_height, sq, twice_return_acceleration;
        if (vz < 0.0f || (twice_return_acceleration = return_acceleration + return_acceleration,
                          remaining_return_height = *(float *)(platform_vars + 0xA0) - moby->pos.z,
                          sq = vz * vz,
                          sq / (twice_return_acceleration + twice_return_acceleration) <
                              remaining_return_height)) {
            float a = vz + return_acceleration;
            float mx = frame_time + frame_time;
            *(float *)(platform_vars + 0xA4) = a;
            if (mx < a)
                *(float *)(platform_vars + 0xA4) = mx;
        } else if (0.0f < remaining_return_height) {
            float a = vz - sq / (remaining_return_height + remaining_return_height);
            *(float *)(platform_vars + 0xA4) = a;
            if (a < 0.0f)
                *(float *)(platform_vars + 0xA4) = 0.0f;
        }
        moby->pos.z += *(float *)(platform_vars + 0xA4);
        if (*(float *)(platform_vars + 0xA0) <= moby->pos.z) {
            moby->pos.z = *(float *)(platform_vars + 0xA0);
            moby->state = 1;
        }
        break;
    }
    }
    fast_vec_sub(position_delta, &moby->pos, previous_position);
    update_entity_matrix(platform_vars + 0x60, position_delta, &moby->rot, &moby->rot);
}
