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
#include "rnc/overlay/shared/math/quad.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"

/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_00296DC0), where it is exact; names translated to the US level program. */

extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void update_moby_rotation_oscillation(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");

void sinking_platform_update(struct Moby *moby) __asm__("FUN_L10_00295c20");

typedef struct SinkingPlatformVars {
    u8  pad_0[0x20];
    s32 health;
    s16 max_health;
    u8  pad_26[0x2];
    u8  hit_weight;
    u8  pad_29[0x15];
    s16 target_flags; /* 0x3E: MobyTargetInfo.flags; target info at pvars+0x20 */
    u8  pad_40[0x20];
    f32 entity_matrix[4];
    u8  pad_70[0x30];
    f32 home_z;
    f32 vertical_velocity;
    f32 bob_phase;
    f32 bob_offset;
    f32 bob_phase_step;
    f32 tilt_phase_1;
    f32 tilt_phase_2;
    s32 timer_BC;
} SinkingPlatformVars;

/* Moby class 1117 */
void sinking_platform_update(struct Moby *moby) {
    enum {
        PLATFORM_STATE_INITIALIZE = 0,
        PLATFORM_STATE_BOBBING = 1,
        PLATFORM_STATE_SINKING_WITH_HERO = 2,
        PLATFORM_STATE_RETURN_TO_ORIGINAL_HEIGHT = 3
    };
    SinkingPlatformVars *platform_vars = (SinkingPlatformVars *)moby->pvars;
    float                previous_position[4];
    float                position_delta[4];
    *(OvlQuad *)previous_position = *(OvlQuad *)&moby->pos;
    if (is_hero_on_moby(moby))
        moby->state = PLATFORM_STATE_SINKING_WITH_HERO;
    fast_dec_timer(&platform_vars->timer_BC);
    switch (moby->state) {
    case PLATFORM_STATE_INITIALIZE:
        platform_vars->hit_weight = 4;
        platform_vars->target_flags = 5;
        platform_vars->health = 0;
        platform_vars->max_health = 0;
        platform_vars->home_z = moby->pos.z;
        platform_vars->bob_phase = random_angle_radians();
        platform_vars->tilt_phase_1 = random_angle_radians();
        {
            float initial_second_rotation_phase = random_angle_radians();
            float bobbing_phase_jitter_limit = frame_time * 0.13962634f;
            platform_vars->tilt_phase_2 = initial_second_rotation_phase;
            platform_vars->bob_phase_step =
                frame_time * 1.5707964f + randf_sym(0.0f, bobbing_phase_jitter_limit);
        }
        moby->state = PLATFORM_STATE_BOBBING;
    case PLATFORM_STATE_BOBBING:
        *(int *)&platform_vars->vertical_velocity = 0;
        update_moby_vertical_oscillation(0.1f,
                                         platform_vars->bob_phase_step,
                                         moby,
                                         &platform_vars->bob_phase,
                                         &platform_vars->bob_offset);
        update_moby_rotation_oscillation(moby,
                                         &platform_vars->tilt_phase_1,
                                         &platform_vars->tilt_phase_2,
                                         0.047123890f,
                                         frame_time * 0.34906584f,
                                         frame_time * 0.5235988f);
        break;
    case PLATFORM_STATE_SINKING_WITH_HERO: {
        float a = platform_vars->vertical_velocity - (frame_time_sq + frame_time_sq);
        float sinking_step_lower_bound = -(frame_time + frame_time);
        platform_vars->vertical_velocity = a;
        if (a < sinking_step_lower_bound)
            platform_vars->vertical_velocity = sinking_step_lower_bound;
        moby->pos.z += platform_vars->vertical_velocity;
        if (is_hero_on_moby(moby) == 0)
            moby->state = PLATFORM_STATE_RETURN_TO_ORIGINAL_HEIGHT;
        if (platform_vars->timer_BC <= 0) {
            allocate_voice_for_target_entry(0, 0, (int)moby);
            platform_vars->timer_BC = random_int_between(0x28, 0x3C);
        }
        break;
    }
    case PLATFORM_STATE_RETURN_TO_ORIGINAL_HEIGHT: {
        float vz = platform_vars->vertical_velocity;
        float return_acceleration = frame_time_sq;
        float remaining_return_height, sq, twice_return_acceleration;
        if (vz < 0.0f || (twice_return_acceleration = return_acceleration + return_acceleration,
                          remaining_return_height = platform_vars->home_z - moby->pos.z,
                          sq = vz * vz,
                          sq / (twice_return_acceleration + twice_return_acceleration) <
                              remaining_return_height)) {
            float a = vz + return_acceleration;
            float mx = frame_time + frame_time;
            platform_vars->vertical_velocity = a;
            if (mx < a)
                platform_vars->vertical_velocity = mx;
        } else if (0.0f < remaining_return_height) {
            float a = vz - sq / (remaining_return_height + remaining_return_height);
            platform_vars->vertical_velocity = a;
            if (a < 0.0f)
                platform_vars->vertical_velocity = 0.0f;
        }
        moby->pos.z += platform_vars->vertical_velocity;
        if (platform_vars->home_z <= moby->pos.z) {
            moby->pos.z = platform_vars->home_z;
            moby->state = PLATFORM_STATE_BOBBING;
        }
        break;
    }
    }
    fast_vec_sub(position_delta, &moby->pos, previous_position);
    update_entity_matrix(platform_vars->entity_matrix, position_delta, &moby->rot, &moby->rot);
}
