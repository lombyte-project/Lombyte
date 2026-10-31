/* Level 13 lift moby class and its mount transform. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"

extern float D_0015ED6C;

extern char D_0013E533[];

extern char D_L13_001670C0[];

typedef struct {
    u8  pad0[0x20];
    u8  path[0x40];
    f32 top;
    f32 bottom;
    f32 speed;
    s32 voice;
    f32 blend;
    f32 blend_speed;
    f32 pulse;
    f32 angle;
    u8  pad80[4];
    s32 timer;
} LiftVarsA6D8;

extern float D_0015ED70;

extern float random_angle_radians(void) __asm__("FUN_00213308");

extern float
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");

void compute_lift_mount_transform(struct Moby *moby,
                                  float       *mount_position,
                                  float       *mount_rotation) __asm__("FUN_L13_0030ad38");

extern void
compute_lift_mount_transform(struct Moby *, float *, float *) __asm__("FUN_L13_0030ad38");

void lift_update(struct Moby *moby) __asm__("FUN_L13_0030a6d8");

/* Moby class 1270 */
void lift_update(struct Moby *moby) {
    enum {
        LIFT_INITIALIZE = 0,
        LIFT_BLEND_TO_MOUNT_TRANSFORM = 1,
        LIFT_MOVE_TO_TOP = 2,
        LIFT_MOVE_TO_BOTTOM = 3,
        LIFT_IDLE_AT_TOP = 4,
        LIFT_IDLE_AT_BOTTOM = 5,
        LIFT_WAIT_AT_TOP_FOR_HERO_TO_LEAVE = 6,
        LIFT_WAIT_AT_BOTTOM_FOR_HERO_TO_LEAVE = 7
    };
    LiftVarsA6D8 *lift_vars = (LiftVarsA6D8 *)moby->pvars;
    float         delta[4];
    float         rot[4];
    float         mount_position_work[4];
    float         mount_transform_work[4];
    float         mount_rotation[4];

    fast_vec_scale(delta, &moby->pos, -1.0f);
    qcopy(rot, &moby->rot);
    moby->rot.y = fast_add_rotations(
        (1.0f - (moby->pos.z - lift_vars->bottom) / (lift_vars->top - lift_vars->bottom)) *
            3.1415927f,
        lift_vars->angle * DEG_TO_RAD);
    switch (moby->state) {
    case LIFT_INITIALIZE:
        lift_vars->pulse = random_angle_radians();
        lift_vars->voice = -1;
        lift_vars->timer = 0;
        moby->state = 4;
        break;
    case LIFT_BLEND_TO_MOUNT_TRANSFORM: {
        float previous_mount_blend = lift_vars->blend;
        advance_accelerated_scalar(&lift_vars->blend,
                                   &lift_vars->blend_speed,
                                   1.0f,
                                   D_0015ED70 * 4.0f,
                                   D_0015ED70 * 4.0f,
                                   D_0015ED6C * 4.0f);
        if (lift_vars->blend < 1.0f) {
            float remaining_blend_ratio = (1.0f - lift_vars->blend) / (1.0f - previous_mount_blend);
            char *mount_rotation_reference_base;
            compute_lift_mount_transform(moby, mount_transform_work, mount_rotation);
            fast_vec_sub(mount_position_work, D_L13_001670C0, mount_transform_work);
            mount_rotation_reference_base = D_L13_001670C0 - 0x140;
            fast_vec_scale(mount_position_work, mount_position_work, remaining_blend_ratio);
            fast_vec_add(mount_transform_work, mount_transform_work, mount_position_work);
            mount_rotation[1] = fast_add_rotations(
                fast_subtract_rotations(*(float *)(mount_rotation_reference_base + 0x154),
                                        mount_rotation[1]) *
                    remaining_blend_ratio,
                mount_rotation[1]);
            mount_rotation[2] = fast_add_rotations(
                fast_subtract_rotations(*(float *)(mount_rotation_reference_base + 0x158),
                                        mount_rotation[2]) *
                    remaining_blend_ratio,
                mount_rotation[2]);
            set_camera_position(mount_transform_work);
            set_camera_rotation(mount_rotation);
        } else {
            u8 s = moby->command;
            moby->command = 1;
            moby->state = s;
        }
        break;
    }
    case LIFT_MOVE_TO_TOP:
    case LIFT_MOVE_TO_BOTTOM: {
        float target;
        if (!voice_is_playing(moby, lift_vars->voice))
            lift_vars->voice = allocate_voice_for_target_entry(0, 4, moby);
        if (moby->command != 0) {
            compute_lift_mount_transform(moby, mount_position_work, mount_transform_work);
            set_camera_position(mount_position_work);
            set_camera_rotation(mount_transform_work);
        }
        if (moby->state == 2)
            target = lift_vars->top;
        else
            target = lift_vars->bottom;
        advance_accelerated_scalar(&moby->pos.z,
                                   &lift_vars->speed,
                                   target,
                                   D_0015ED70 * 8.0f,
                                   D_0015ED70 * 8.0f,
                                   D_0015ED6C * 40.0f);
        if (absolute_float(moby->pos.z - target) == 0.0f) {
            lift_vars->timer = scale_game_frames(0x46);
            if (moby->state == 2)
                moby->state = 6;
            else
                moby->state = 7;
            if (moby->command != 0)
                jet_end_mount(2);
        }
        break;
    }
    case LIFT_WAIT_AT_TOP_FOR_HERO_TO_LEAVE:
    case LIFT_WAIT_AT_BOTTOM_FOR_HERO_TO_LEAVE:
        if (lift_vars->timer != 0 && fast_dec_timer(&lift_vars->timer))
            hero_set_state(0, 0);
        if (voice_is_playing(moby, lift_vars->voice)) {
            release_voice_slot(lift_vars->voice);
            lift_vars->voice = -1;
        }
        if (hero.ground_moby != moby) {
            if (moby->state == 7)
                moby->state = 5;
            else
                moby->state = 4;
        }
        break;
    case LIFT_IDLE_AT_TOP:
    case LIFT_IDLE_AT_BOTTOM: {
        int i;
        lift_vars->pulse = fast_add_rotations(lift_vars->pulse, D_0015ED6C * 6.2831855f);
        i = truncate_float_to_s32((fast_sin(lift_vars->pulse) * 4.0f - 3.0f) * 128.0f);
        if (i > 0x80)
            i = 0x80;
        else if (i < 0x20)
            i = 0x20;
        moby->glow_color = ((i << 16) | 0x80000000) | (i << 8) | i;
        if (hero.ground_moby == moby && hero.air_frames.s == 0 &&
            vector_distance_xy(&hero.motion.pos, &moby->pos) < 0.5f) {
            moby->glow_color = 0x80208020;
            if (moby->state == 5)
                moby->command = 2;
            else
                moby->command = 3;
            moby->state = 1;
            hero_set_state(0x72, 1);
            jet_mount(D_L13_001670C0, D_L13_001670C0 + 0x10, 1, 0, 0);
            lift_vars->blend = 0.0f;
            break;
        }
        if (moby->state == 4) {
            float d = vector_distance_xy(&moby->pos, &hero.motion.pos);
            if (absolute_float(hero.ground_z.f - lift_vars->bottom) < 2.0f && d < 32.0f &&
                d > 5.0f) {
                moby->command = 0;
                moby->state = 3;
                lift_vars->speed = -(D_0015ED6C * 5.0f);
            }
        } else if (moby->state == 5) {
            float d = vector_distance_xy(&moby->pos, &hero.motion.pos);
            if (absolute_float(hero.ground_z.f - lift_vars->top) < 2.0f && d < 32.0f && d > 5.0f) {
                moby->command = 0;
                moby->state = 2;
                lift_vars->speed = D_0015ED6C * 5.0f;
            }
        }
        break;
    }
    }
    fast_vec_add(delta, delta, &moby->pos);
    update_entity_matrix(lift_vars->path, delta, rot, &moby->rot);
}

void compute_lift_mount_transform(struct Moby *moby, float *mount_position, float *mount_rotation) {
    float mount_offset_angle, k;
    char *mount_facing_target_base;
    clear_u64_value(mount_rotation);
    k = 2.25f;
    if ((moby->command >= 2 && (moby->command & 1)) || (moby->state >= 2 && (moby->state & 1))) {
        mount_offset_angle = -1.5707964f;
        mount_rotation[0] = moby->rot.y;
    } else {
        mount_offset_angle = 1.5707964f;
        mount_rotation[0] = -moby->rot.y;
    }
    mount_position[0] = fast_cos(mount_offset_angle) * k;
    mount_position[1] = fast_sin(mount_offset_angle) * k;
    mount_position[2] = 1.5f;
    transform_vector_by_basis(mount_position, mount_position, &moby->rot_row0);
    fast_vec_add(mount_position, mount_position, &moby->pos);
    mount_facing_target_base = D_0013E533 + 0xE1D;
    mount_rotation[2] =
        fast_arc_tan(*(float *)(mount_facing_target_base + 0xD0) - mount_position[0],
                     *(float *)(mount_facing_target_base + 0xD4) - mount_position[1]);
}
