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
    s32 unk20;
    s16 unk24;
    u8  pad_26[0x2];
    u8  unk28;
    u8  pad_29[0x15];
    s16 unk3E;
    u8  pad_40[0x20];
    f32 unk60[4];
    u8  pad_70[0x30];
    f32 unkA0;
    f32 unkA4;
    f32 unkA8;
    f32 unkAC;
    f32 unkB0;
    f32 unkB4;
    f32 unkB8;
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
        moby->state = 2;
    fast_dec_timer(&platform_vars->timer_BC);
    switch (moby->state) {
    case PLATFORM_STATE_INITIALIZE:
        platform_vars->unk28 = 4;
        platform_vars->unk3E = 5;
        platform_vars->unk20 = 0;
        platform_vars->unk24 = 0;
        platform_vars->unkA0 = moby->pos.z;
        platform_vars->unkA8 = random_angle_radians();
        platform_vars->unkB4 = random_angle_radians();
        {
            float initial_second_rotation_phase = random_angle_radians();
            float bobbing_phase_jitter_limit = frame_time * 0.13962634f;
            platform_vars->unkB8 = initial_second_rotation_phase;
            platform_vars->unkB0 =
                frame_time * 1.5707964f + randf_sym(0.0f, bobbing_phase_jitter_limit);
        }
        moby->state = 1;
    case PLATFORM_STATE_BOBBING:
        *(int *)&platform_vars->unkA4 = 0;
        update_moby_vertical_oscillation(
            0.1f, platform_vars->unkB0, moby, &platform_vars->unkA8, &platform_vars->unkAC);
        update_moby_rotation_oscillation(moby,
                                         &platform_vars->unkB4,
                                         &platform_vars->unkB8,
                                         0.047123890f,
                                         frame_time * 0.34906584f,
                                         frame_time * 0.5235988f);
        break;
    case PLATFORM_STATE_SINKING_WITH_HERO: {
        float a = platform_vars->unkA4 - (frame_time_sq + frame_time_sq);
        float sinking_step_lower_bound = -(frame_time + frame_time);
        platform_vars->unkA4 = a;
        if (a < sinking_step_lower_bound)
            platform_vars->unkA4 = sinking_step_lower_bound;
        moby->pos.z += platform_vars->unkA4;
        if (is_hero_on_moby(moby) == 0)
            moby->state = 3;
        if (platform_vars->timer_BC <= 0) {
            allocate_voice_for_target_entry(0, 0, (int)moby);
            platform_vars->timer_BC = random_int_between(0x28, 0x3C);
        }
        break;
    }
    case PLATFORM_STATE_RETURN_TO_ORIGINAL_HEIGHT: {
        float vz = platform_vars->unkA4;
        float return_acceleration = frame_time_sq;
        float remaining_return_height, sq, twice_return_acceleration;
        if (vz < 0.0f || (twice_return_acceleration = return_acceleration + return_acceleration,
                          remaining_return_height = platform_vars->unkA0 - moby->pos.z,
                          sq = vz * vz,
                          sq / (twice_return_acceleration + twice_return_acceleration) <
                              remaining_return_height)) {
            float a = vz + return_acceleration;
            float mx = frame_time + frame_time;
            platform_vars->unkA4 = a;
            if (mx < a)
                platform_vars->unkA4 = mx;
        } else if (0.0f < remaining_return_height) {
            float a = vz - sq / (remaining_return_height + remaining_return_height);
            platform_vars->unkA4 = a;
            if (a < 0.0f)
                platform_vars->unkA4 = 0.0f;
        }
        moby->pos.z += platform_vars->unkA4;
        if (platform_vars->unkA0 <= moby->pos.z) {
            moby->pos.z = platform_vars->unkA0;
            moby->state = 1;
        }
        break;
    }
    }
    fast_vec_sub(position_delta, &moby->pos, previous_position);
    update_entity_matrix(platform_vars->unk60, position_delta, &moby->rot, &moby->rot);
}
