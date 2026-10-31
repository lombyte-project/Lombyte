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

extern int   is_hero_on_moby(void *) __asm__("FUN_L01_00277fb8");
extern int   allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void  update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void  mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern f32   random_float_between(f32, f32) __asm__("FUN_002132a8");
extern int   allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
void         mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern float random_float_between(float, float) __asm__("FUN_002132a8");

/* Falling leaf: drops off its branch after a random delay, swaying, then falls faster and vanishes near the ground. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00312FA8), where it is exact; names translated to the US level program. */
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
s32         allocate_voice_for_target_entry(s32   entry_index,
                                            s32   flags,
                                            void *target) __asm__("FUN_0022da68");
void        hero_falling_platform_update(char *moby) __asm__("FUN_L07_00311bc8");

/* Moby class 1080 */
void hero_falling_platform_update(char *moby) {
    enum {
        LEAF_INITIALIZE = 0,
        LEAF_WAIT_FOR_HERO_CONTACT = 1,
        LEAF_TIMED_SWAY_AND_DESCENT = 2,
        LEAF_ACCELERATED_FALL = 3
    };
    char *moby_data;
    float previous_position[4];
    float position_delta[4];
    float tilt_descent_work_value;
    float gravity_step;
    if (moby == 0)
        return;
    moby_data = ((char *)MOBY(moby)->pvars);
    if (moby_data == 0)
        return;
    qcopy(previous_position, moby + 0x10);
    switch (((unsigned char *)moby)[0x20]) {
    case LEAF_INITIALIZE:
        *(int *)(moby_data + 0x20) = 0;
        *(short *)(moby_data + 0x24) = 0;
        moby_data[0x28] = 4;
        *(short *)(moby_data + 0x3E) = 0xD;
        set_moby_color(moby, 0x50, 0x40, 0x10);
        MOBY(moby)->rot.x += random_float_between(-0.034906585f, 0.034906585f);
        MOBY(moby)->rot.y += random_float_between(-0.034906585f, 0.034906585f);
        MOBY(moby)->pos.z += random_float_between(-0.02f, 0.02f);
        moby[0x20] = 1;
        break;
    case LEAF_WAIT_FOR_HERO_CONTACT:
        if (is_hero_on_moby(moby)) {
            *(float *)(moby_data + 0xA8) = MOBY(moby)->rot.x;
            *(float *)(moby_data + 0xAC) = MOBY(moby)->rot.y;
            *(short *)(moby_data + 0xA0) = scale_game_frames(0x1E);
            if (*(short *)(moby_data + 0xA2) != -1)
                allocate_voice_for_target_entry(*(short *)(moby_data + 0xA2), 0, (int)moby);
            moby[0x20] = 2;
        }
        break;
    case LEAF_TIMED_SWAY_AND_DESCENT:
        MOBY(moby)->rot.x = fast_sin(normalize_angle(
                                convert_integer_to_float(*(short *)(moby_data + 0xA0)) * 0.75f)) *
                            DEG_TO_RAD;
        {
            float c = fast_cos(
                normalize_angle(convert_integer_to_float(*(short *)(moby_data + 0xA0)) * 0.89f));
            float descent_frame_scale = frame_scale;
            MOBY(moby)->rot.y = c * DEG_TO_RAD;
            MOBY(moby)->pos.z -= descent_frame_scale * 0.002f;
        }
        if (fast_dec_timer_s16(moby_data + 0xA0)) {
            *(float *)(moby_data + 0xA4) = frame_scale * 0.002f;
            moby[0x20] = 3;
        }
        break;
    case LEAF_ACCELERATED_FALL:
        tilt_descent_work_value = (1.3962634f - MOBY(moby)->rot.x) * 0.05f;
        MOBY(moby)->rot.x += tilt_descent_work_value;
        tilt_descent_work_value = fast_sin(tilt_descent_work_value);
        gravity_step = frame_time_sq * 9.8f;
        MOBY(moby)->pos.z -= tilt_descent_work_value + tilt_descent_work_value;
        *(float *)(moby_data + 0xA4) += gravity_step;
        MOBY(moby)->pos.z -= *(float *)(moby_data + 0xA4);
        if (MOBY(moby)->pos.z < 2.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case 4:
        break;
    }
    fast_vec_sub(position_delta, moby + 0x10, previous_position);
    update_entity_matrix(moby_data + 0x60, position_delta, moby + 0x40, moby + 0x40);
}
