/* Level 06 moby class 0x41e: split door; its two halves (the second is class 0x41f) slide apart when the hero enters its clip volume. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/math/interpolation/advance_accelerated_scalar.h"
#include "rnc/rendering/clip_volume.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002F6790), where it is exact; names translated to the US level program. */
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");

#include "qcopy.h"

/* raise each listed moby's position z by a fixed amount and notify it */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FC4C0), where it is exact; names translated to the US level program. */
extern float random_float_between(float, float) __asm__("FUN_002132a8");

/* Registers the moby's draw callbacks, choosing by its four slot ids. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FE4B8), where it is exact; names translated to the US level program. */

/* Sparks thrown off the swinging hazard while the camera is near it. */
extern f32   random_float_between(f32, f32) __asm__("FUN_002132a8");
extern char *create_moby(int);

#include "sda.h"

extern void  set_moby_list_state(int, int) __asm__("FUN_L01_0026e0e0");
extern float moby_pair_parent_fixed_distance_gp __asm__("D_L06_00161F1C") __attribute__((sda));
extern float moby_pair_child_fixed_distance_gp __asm__("D_L06_00161F20") __attribute__((sda));
extern float moby_pair_parent_phase_target_distance_gp __asm__("D_L06_00161F24")
    __attribute__((sda));
extern float moby_pair_child_phase_target_distance_gp __asm__("D_L06_00161F28")
    __attribute__((sda));
extern float moby_pair_parent_phase_two_target_rotation_y_gp __asm__("D_L06_00161F2C")
    __attribute__((sda));
extern float moby_pair_child_phase_two_target_rotation_y_gp __asm__("D_L06_00161F30")
    __attribute__((sda));

#include "rnc/overlay/l06/mobys/split_door_data/update_split_door_2.h"

extern float moby_pair_phase_distance_max_speed_gp __asm__("D_L06_00161F3C") __attribute__((sda));

#include "rnc/overlay/l06/mobys/split_door_data/split_door_update.h"

extern float moby_pair_phase_rotation_y_max_speed_degrees_gp __asm__("D_L06_00161F48")
    __attribute__((sda));
extern int moby_pair_phase_one_duration_min_frames_gp __asm__("D_L06_00161F4C")
    __attribute__((sda));
extern int moby_pair_phase_one_duration_max_frames_gp __asm__("D_L06_00161F50")
    __attribute__((sda));
extern int moby_pair_phase_two_duration_min_frames_gp __asm__("D_L06_00161F54")
    __attribute__((sda));
extern int moby_pair_phase_two_duration_max_frames_gp __asm__("D_L06_00161F58")
    __attribute__((sda));
extern float moby_pair_reset_enabled_gp __asm__("D_L06_00161F5C") __attribute__((sda));
void         split_door_update(char *moby) __asm__("FUN_L06_002fbfb0");

typedef struct SplitDoorVars {
    u8    pad_0[0x4];
    s32   open_mode;
    u8    slide_speed;
    u8    pad_9[0x3];
    u8    turn_speed;
    u8    pad_D[0x3];
    u8    home_position;
    u8    pad_11[0xF];
    f32   home_rotation[4];
    f32   split_axis[4];
    char *other_half;
    u8    other_slide_speed;
    u8    pad_45[0x3];
    u8    other_turn_speed;
    u8    pad_49[0x3];
    s32   timer_4C;
} SplitDoorVars;

/* Moby class 1054 */
void split_door_update(char *moby) {
    enum {
        INITIALIZE_CHILD_AND_TRANSFORMS = 0,
        WAIT_FOR_HERO_IN_CLIP_VOLUME = 1,
        MOVE_PAIR_TO_FIXED_OFFSETS = 3,
        ANIMATE_PAIR_WITH_TIMED_PHASES = 4,
        WAIT_FOR_RESET = 5
    };
    enum {
        SPLIT_DOOR_PHASE_TARGET_CURRENT_TRANSFORMS = 1,
        SPLIT_DOOR_PHASE_TARGET_CONFIGURED_TRANSFORMS = 2
    };
    enum {
        SPLIT_DOOR_STATE_NO_UPDATE = 2
    };
    SplitDoorVars *pair_transform_vars = (SplitDoorVars *)MOBY(moby)->pvars;
    char          *child;
    float          position_offset[4];
    float          parent_distance, child_distance;
    float          phase_a, phase_b;
    float          target_delta;
    float          target_a, target_b, target_rot_a, target_rot_b;
    float acceleration, max_speed, damping, rotation_acceleration, rotation_speed, rotation_damping;
    int   next_timer;
    unsigned char next_phase;
    int           phase;

    switch ((unsigned char)MOBY(moby)->state) {
    case INITIALIZE_CHILD_AND_TRANSFORMS:
        qcopy(&pair_transform_vars->home_position, moby + 0x10);
        qcopy(pair_transform_vars->home_rotation, moby + 0x40);
        qcopy(pair_transform_vars->split_axis, moby + 0xe0);
        pair_transform_vars->other_half = (char *)create_moby(0x41f);
        *(unsigned short *)(pair_transform_vars->other_half + 0x32) = 0x40;
        (pair_transform_vars->other_half)[0x31] = 1;
        *(long *)(pair_transform_vars->other_half + 0x38) = ((long)MOBY(moby)->spawn_frame);
        *(unsigned short *)(pair_transform_vars->other_half + 0x34) = MOBY(moby)->flags;
        child = pair_transform_vars->other_half + 0x10;
        qcopy(child, moby + 0x10);
        qcopy(child + 0x30, moby + 0x40);
        if (*(int *)pair_transform_vars >= 0) {
            MOBY(moby)->state = WAIT_FOR_HERO_IN_CLIP_VOLUME;
            return;
        }
        if (pair_transform_vars->open_mode == 2) {
            MOBY(moby)->state = 2;
            return;
        }
        fast_vec_normalize(
            position_offset, pair_transform_vars->split_axis, moby_pair_parent_fixed_distance_gp);
        fast_vec_add(moby + 0x10, &pair_transform_vars->home_position, position_offset);
        fast_vec_normalize(
            position_offset, pair_transform_vars->split_axis, -moby_pair_child_fixed_distance_gp);
        fast_vec_add(pair_transform_vars->other_half + 0x10,
                     &pair_transform_vars->home_position,
                     position_offset);
        update_entity(pair_transform_vars->other_half);
        MOBY(moby)->state = WAIT_FOR_RESET;
        return;
    case WAIT_FOR_HERO_IN_CLIP_VOLUME:
        if (!is_point_inside_clip_volume(&hero.motion.pos, *(int *)pair_transform_vars))
            return;
        if (pair_transform_vars->open_mode == 1) {
            MOBY(moby)->state = ANIMATE_PAIR_WITH_TIMED_PHASES;
            pair_transform_vars->timer_4C = scale_game_frames(0x14);
        } else if ((unsigned char)MOBY(moby)->group != 0xff) {
            set_moby_list_state((unsigned char)MOBY(moby)->group, 3);
        } else {
            MOBY(moby)->state = MOVE_PAIR_TO_FIXED_OFFSETS;
        }
        allocate_voice_for_target_entry(0, 0, (int)pair_transform_vars->other_half);
        return;
    case MOVE_PAIR_TO_FIXED_OFFSETS:
        target_delta = fast_vec_dist(moby + 0x10, &pair_transform_vars->home_position);
        child = pair_transform_vars->other_half;
        parent_distance = target_delta;
        child_distance = fast_vec_dist(child + 0x10, &pair_transform_vars->home_position);
        advance_accelerated_scalar(&parent_distance,
                                   &((float *)pair_transform_vars)[2],
                                   moby_pair_parent_fixed_distance_gp,
                                   frame_time_sq * 10.0f,
                                   frame_time_sq * 20.0f,
                                   frame_time * 5.0f);
        advance_accelerated_scalar(&child_distance,
                                   &((float *)pair_transform_vars)[0x11],
                                   moby_pair_child_fixed_distance_gp,
                                   frame_time_sq * 10.0f,
                                   frame_time_sq * 20.0f,
                                   frame_time * 5.0f);
        fast_vec_normalize(position_offset, pair_transform_vars->split_axis, parent_distance);
        fast_vec_add(moby + 0x10, &pair_transform_vars->home_position, position_offset);
        fast_vec_normalize(position_offset, pair_transform_vars->split_axis, -child_distance);
        fast_vec_add(pair_transform_vars->other_half + 0x10,
                     &pair_transform_vars->home_position,
                     position_offset);
        update_entity(pair_transform_vars->other_half);
        if (absolute_float(parent_distance - moby_pair_parent_fixed_distance_gp) < 0.01f &&
            absolute_float(child_distance - moby_pair_child_fixed_distance_gp) < 0.01f)
            MOBY(moby)->state = WAIT_FOR_RESET;
        return;
    case ANIMATE_PAIR_WITH_TIMED_PHASES:
        target_delta = fast_vec_dist(moby + 0x10, &pair_transform_vars->home_position);
        child = pair_transform_vars->other_half;
        phase_a = target_delta;
        phase_b = fast_vec_dist(child + 0x10, &pair_transform_vars->home_position);
        phase = (unsigned char)MOBY(moby)->command;
        if (phase == SPLIT_DOOR_PHASE_TARGET_CURRENT_TRANSFORMS)
            goto phase_one;
        if (phase < SPLIT_DOOR_PHASE_TARGET_CONFIGURED_TRANSFORMS)
            goto phase_default;
        if (phase == SPLIT_DOOR_PHASE_TARGET_CONFIGURED_TRANSFORMS)
            goto phase_two;

    phase_default:
        target_a = moby_pair_parent_phase_target_distance_gp;
        target_rot_a = MOBY(moby)->rot.y;
        target_rot_b = target_rot_a;
        target_b = moby_pair_child_phase_target_distance_gp;
        acceleration = 10.0f;
        max_speed = 20.0f;
        damping = 5.0f;
        rotation_acceleration = 10.0f;
        rotation_speed = 10.0f;
        rotation_damping = 5.0f;
        next_phase = SPLIT_DOOR_PHASE_TARGET_CURRENT_TRANSFORMS;
        next_timer = 30;
        goto phase_join;
    phase_one:
        target_rot_a = MOBY(moby)->rot.y;
        target_rot_b = *(float *)(pair_transform_vars->other_half + 0x44);
        target_a = phase_a;
        target_b = phase_b;
        acceleration = moby_pair_phase_distance_acceleration_gp;
        max_speed = moby_pair_phase_distance_deceleration_gp;
        damping = moby_pair_phase_distance_max_speed_gp;
        rotation_acceleration = moby_pair_phase_rotation_y_acceleration_degrees_gp;
        rotation_speed = moby_pair_phase_rotation_y_deceleration_degrees_gp;
        rotation_damping = moby_pair_phase_rotation_y_max_speed_degrees_gp;
        next_phase = SPLIT_DOOR_PHASE_TARGET_CONFIGURED_TRANSFORMS;
        next_timer = truncate_float_to_s32(
            random_float_between((float)moby_pair_phase_two_duration_min_frames_gp,
                                 (float)moby_pair_phase_two_duration_max_frames_gp));
        goto phase_join;
    phase_two:
        target_a = moby_pair_parent_phase_target_distance_gp;
        target_b = moby_pair_child_phase_target_distance_gp;
        target_rot_a = moby_pair_parent_phase_two_target_rotation_y_gp;
        target_rot_b = moby_pair_child_phase_two_target_rotation_y_gp;
        acceleration = moby_pair_phase_distance_acceleration_gp;
        max_speed = moby_pair_phase_distance_deceleration_gp;
        damping = moby_pair_phase_distance_max_speed_gp;
        rotation_acceleration = moby_pair_phase_rotation_y_acceleration_degrees_gp;
        rotation_speed = moby_pair_phase_rotation_y_deceleration_degrees_gp;
        rotation_damping = moby_pair_phase_rotation_y_max_speed_degrees_gp;
        next_phase = SPLIT_DOOR_PHASE_TARGET_CURRENT_TRANSFORMS;
        next_timer = truncate_float_to_s32(
            random_float_between((float)moby_pair_phase_one_duration_min_frames_gp,
                                 (float)moby_pair_phase_one_duration_max_frames_gp));
    phase_join:
        advance_accelerated_scalar(&phase_a,
                                   &((float *)pair_transform_vars)[2],
                                   target_a,
                                   acceleration * frame_time_sq,
                                   max_speed * frame_time_sq,
                                   damping * frame_time);
        advance_accelerated_scalar(&phase_b,
                                   &((float *)pair_transform_vars)[0x11],
                                   target_b,
                                   acceleration * frame_time_sq,
                                   max_speed * frame_time_sq,
                                   damping * frame_time);
        advance_accelerated_scalar((float *)&MOBY(moby)->rot.y,
                                   &((float *)pair_transform_vars)[3],
                                   target_rot_a,
                                   rotation_acceleration * DEG_TO_RAD * frame_time_sq,
                                   rotation_speed * DEG_TO_RAD * frame_time_sq,
                                   rotation_damping * DEG_TO_RAD * frame_time);
        advance_accelerated_scalar((float *)(pair_transform_vars->other_half + 0x44),
                                   &((float *)pair_transform_vars)[0x12],
                                   target_rot_b,
                                   rotation_acceleration * DEG_TO_RAD * frame_time_sq,
                                   rotation_speed * DEG_TO_RAD * frame_time_sq,
                                   rotation_damping * DEG_TO_RAD * frame_time);
        fast_vec_normalize(position_offset, pair_transform_vars->split_axis, phase_a);
        fast_vec_add(moby + 0x10, &pair_transform_vars->home_position, position_offset);
        fast_vec_normalize(position_offset, pair_transform_vars->split_axis, -phase_b);
        fast_vec_add(pair_transform_vars->other_half + 0x10,
                     &pair_transform_vars->home_position,
                     position_offset);
        update_entity(pair_transform_vars->other_half);
        if (!fast_dec_timer(&pair_transform_vars->timer_4C))
            return;
        MOBY(moby)->command = next_phase;
        pair_transform_vars->timer_4C = scale_game_frames(next_timer);
        target_delta = moby_pair_parent_phase_target_distance_gp;
        if (absolute_float(phase_a - target_delta) < 0.01f) {
            if (absolute_float(phase_b - moby_pair_child_phase_target_distance_gp) < 0.01f)
                MOBY(moby)->state = WAIT_FOR_RESET;
        }
        return;
    case WAIT_FOR_RESET:
        if (moby_pair_reset_enabled_gp == 0.0f)
            return;
        MOBY(moby)->command = 0;
        MOBY(moby)->state = WAIT_FOR_HERO_IN_CLIP_VOLUME;
        qcopy(moby + 0x10, &pair_transform_vars->home_position);
        qcopy(moby + 0x40, pair_transform_vars->home_rotation);
        child = pair_transform_vars->other_half + 0x10;
        qcopy(child, moby + 0x10);
        qcopy(child + 0x30, moby + 0x40);
        *(volatile float *)(&pair_transform_vars->other_turn_speed) = 0.0f;
        *(volatile float *)(&pair_transform_vars->slide_speed) = 0.0f;
        *(volatile float *)(&pair_transform_vars->turn_speed) = 0.0f;
        *(volatile float *)(&pair_transform_vars->other_slide_speed) = 0.0f;
        return;
    }
}
