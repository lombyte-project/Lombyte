/* Level 07 moby class 0x438: hero triggered falling platform. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "qcopy.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"

extern int   allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void  update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern f32   random_float_between(f32, f32) __asm__("FUN_002132a8");
extern int   allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern float random_float_between(float, float) __asm__("FUN_002132a8");

/* Falling leaf: drops off its branch after a random delay, swaying, then falls faster and vanishes near the ground. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00312FA8), where it is exact; names translated to the US level program. */
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
s32         allocate_voice_for_target_entry(s32   entry_index,
                                            s32   flags,
                                            void *target) __asm__("FUN_0022da68");
void        hero_falling_platform_update(char *moby) __asm__("FUN_L07_00311bc8");

typedef struct HeroFallingPlatformVars {
    u8  pad_0[0x20];
    s32 unk20;
    s16 unk24;
    u8  pad_26[0x2];
    u8  unk28;
    u8  pad_29[0x15];
    s16 unk3E;
    u8  pad_40[0x20];
    f32 transform[16];
    s16 timer_A0;
    s16 contact_sound_entry_index;
    f32 fall_speed;
    f32 unkA8;
    f32 unkAC;
} HeroFallingPlatformVars;

/* Moby class 1080 */
void hero_falling_platform_update(char *moby) {
    enum {
        LEAF_INITIALIZE = 0,
        LEAF_WAIT_FOR_HERO_CONTACT = 1,
        LEAF_TIMED_SWAY_AND_DESCENT = 2,
        LEAF_ACCELERATED_FALL = 3
    };
    HeroFallingPlatformVars *moby_data;
    float                    previous_position[4];
    float                    position_delta[4];
    float                    tilt_descent_work_value;
    float                    gravity_step;
    if (moby == 0)
        return;
    moby_data = (HeroFallingPlatformVars *)MOBY(moby)->pvars;
    if (moby_data == 0)
        return;
    qcopy(previous_position, moby + 0x10);
    switch (((unsigned char *)moby)[0x20]) {
    case LEAF_INITIALIZE:
        moby_data->unk20 = 0;
        moby_data->unk24 = 0;
        moby_data->unk28 = 4;
        moby_data->unk3E = 0xD;
        set_moby_color(moby, 0x50, 0x40, 0x10);
        MOBY(moby)->rot.x += random_float_between(-0.034906585f, 0.034906585f);
        MOBY(moby)->rot.y += random_float_between(-0.034906585f, 0.034906585f);
        MOBY(moby)->pos.z += random_float_between(-0.02f, 0.02f);
        MOBY(moby)->state = LEAF_WAIT_FOR_HERO_CONTACT;
        break;
    case LEAF_WAIT_FOR_HERO_CONTACT:
        if (is_hero_on_moby(moby)) {
            moby_data->unkA8 = MOBY(moby)->rot.x;
            moby_data->unkAC = MOBY(moby)->rot.y;
            moby_data->timer_A0 = scale_game_frames(0x1E);
            if (moby_data->contact_sound_entry_index != -1)
                allocate_voice_for_target_entry(moby_data->contact_sound_entry_index, 0, (int)moby);
            MOBY(moby)->state = LEAF_TIMED_SWAY_AND_DESCENT;
        }
        break;
    case LEAF_TIMED_SWAY_AND_DESCENT:
        MOBY(moby)->rot.x =
            fast_sin(normalize_angle(convert_integer_to_float(moby_data->timer_A0) * 0.75f)) *
            DEG_TO_RAD;
        {
            float c =
                fast_cos(normalize_angle(convert_integer_to_float(moby_data->timer_A0) * 0.89f));
            float descent_frame_scale = frame_scale;
            MOBY(moby)->rot.y = c * DEG_TO_RAD;
            MOBY(moby)->pos.z -= descent_frame_scale * 0.002f;
        }
        if (fast_dec_timer_s16(&moby_data->timer_A0)) {
            moby_data->fall_speed = frame_scale * 0.002f;
            MOBY(moby)->state = LEAF_ACCELERATED_FALL;
        }
        break;
    case LEAF_ACCELERATED_FALL:
        tilt_descent_work_value = (1.3962634f - MOBY(moby)->rot.x) * 0.05f;
        MOBY(moby)->rot.x += tilt_descent_work_value;
        tilt_descent_work_value = fast_sin(tilt_descent_work_value);
        gravity_step = frame_time_sq * 9.8f;
        MOBY(moby)->pos.z -= tilt_descent_work_value + tilt_descent_work_value;
        moby_data->fall_speed += gravity_step;
        MOBY(moby)->pos.z -= moby_data->fall_speed;
        if (MOBY(moby)->pos.z < 2.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case 4:
        break;
    }
    fast_vec_sub(position_delta, moby + 0x10, previous_position);
    update_entity_matrix(moby_data->transform, position_delta, moby + 0x40, moby + 0x40);
}
