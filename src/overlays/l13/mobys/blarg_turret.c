/* Level 13 Blarg turret moby class: targeting and projectiles. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/overlay/l13/mobys/blarg_turret_globals.h"

#ifndef NOT_SDA
#define NOT_SDA __attribute__((section(".data")))
#endif
#ifndef MACRO_ADDR
#define MACRO_ADDR __attribute__((section(".sdata")))
#endif
#include "qcopy.h"
#include "rnc/overlay/l13/mobys/gemlik_projectile.h"

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V4_2B5368;

typedef struct Cls {
    char  pad0[0x24];
    float scale;
} Cls;

typedef struct Moby_2B5368 {
    char          pad0[0x10];
    V4_2B5368     pos;
    signed char   state;
    char          pad21[3];
    Cls          *cls;
    char          pad28[4];
    float         scale;
    char          pad30[0x10];
    V4_2B5368     rot;
    char          pad50[3];
    unsigned char b53;
    char          pad54[0x24];
    void         *data;
    char          pad7C[0x2A];
    short         cls_id;
    char          padA8[0x18];
    V4_2B5368     mat[3];
} Moby_2B5368;

typedef struct Data {
    char         pad0[0x40];
    Moby_2B5368 *target;
    char         pad44[0xC];
    int          volume;
    float        f54;
    char         pad58[4];
    Moby_2B5368 *gun;
    float        f60;
    float        f64;
    float        f68;
    float        f6C;
    char         fx[0x10];
} Data;

extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");

extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            int,
                            float,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

extern void FUN_L13_002b48d8_2b0e80(void *) __asm__("FUN_L13_002b48d8");

extern void *FUN_L13_002b4958_2b0e80(void *) __asm__("FUN_L13_002b4958");

extern void FUN_L00_00263ac8_2b0e80(float, void *, int, void *) __asm__("FUN_L00_00263ac8");

extern float approach_angle(void *, float, void *, float, float, float) __asm__("FUN_L00_0025be00");

extern void FUN_L13_002b47a0_2b0e80(void *) __asm__("FUN_L13_002b47a0");

extern void mark_moby_for_removal_cf(void *, int, int) __asm__("FUN_L00_00257470");

extern unsigned char D_0013F420_2b0e80[] __asm__("D_0013F420");

void blarg_turret_update(Moby_2B5368 *m) __asm__("FUN_L13_002b41b8");

/* Moby class 29 */
void blarg_turret_update(Moby_2B5368 *turret_moby) {
    enum {
        BLARG_TURRET_STATE_INITIALIZE = 0,
        BLARG_TURRET_STATE_WAIT_FOR_TARGET = 1,
        BLARG_TURRET_STATE_TRACK_TARGET = 3,
        BLARG_TURRET_STATE_DESTROY = 4,
        BLARG_TURRET_STATE_DESTRUCTION_DELAY = 5
    };
    Data        *data = turret_moby->data;
    Moby_2B5368 *target_moby;
    Moby_2B5368 *gun;
    float        target_yaw_scratch;

    FUN_L13_002b48d8_2b0e80(turret_moby);
    target_moby = data->target;
    if (data->gun != 0 && data->gun->cls_id == 0x24 && data->gun->state >= 0) {
        FUN_L00_00263ac8_2b0e80(2.7f, data->gun, 1, data->fx);
    }
    switch ((unsigned char)turret_moby->state) {
    case BLARG_TURRET_STATE_INITIALIZE:
        turret_moby->scale = turret_moby->cls->scale * 1.5f;
        data->gun = FUN_L13_002b4958_2b0e80(turret_moby);
        data->f54 = turret_moby->rot.z;
        if (turret_moby->b53 != 0) {
            blend_moby_animation(turret_moby, 0, 0, 10);
        }
        turret_moby->state = 1;
        break;
    case BLARG_TURRET_STATE_WAIT_FOR_TARGET:
        target_yaw_scratch =
            fast_difference_between_rotations(fast_arc_tan(target_moby->pos.x - turret_moby->pos.x,
                                                           target_moby->pos.y - turret_moby->pos.y),
                                              data->f54);
        if (target_yaw_scratch < 1.5707964f &&
            vector_distance_xy(&turret_moby->pos, &target_moby->pos) < 20.0f &&
            (data->volume == -1 ||
             is_point_inside_clip_volume(D_0013F420_2b0e80, data->volume) != 0)) {
            data->f68 = 8.0f;
            if (turret_moby->b53 != 1) {
                int animation_start_frame = random_int_between(0, 3);
                blend_moby_animation(
                    turret_moby, 1, animation_start_frame, random_int_between(7, 13));
            }
            turret_moby->state = 3;
        }
        approach_angle(&turret_moby->rot.z,
                       data->f54,
                       &data->f60,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 12.566371f);
        if (data->gun == 0 || data->gun->cls_id != 0x24 ||
            (unsigned char)data->gun->state == 0xFE || (unsigned char)data->gun->state == 0xFD ||
            (unsigned char)data->gun->state == 2) {
            if (turret_moby->b53 != 0) {
                blend_moby_animation(turret_moby, 0, 0, random_int_between(7, 13));
            }
            data->f64 = 1.0f;
            turret_moby->state = 5;
        }
        break;
    case BLARG_TURRET_STATE_TRACK_TARGET:
        target_yaw_scratch =
            fast_difference_between_rotations(fast_arc_tan(target_moby->pos.x - turret_moby->pos.x,
                                                           target_moby->pos.y - turret_moby->pos.y),
                                              data->f54);
        if (target_yaw_scratch >= 1.5707964f ||
            vector_distance_xy(&turret_moby->pos, &target_moby->pos) > 22.0f ||
            (data->volume != -1 &&
             is_point_inside_clip_volume(D_0013F420_2b0e80, data->volume) == 0)) {
            if (turret_moby->b53 != 0) {
                blend_moby_animation(turret_moby, 0, 0, random_int_between(7, 13));
            }
            turret_moby->state = 1;
        } else {
            float yaw_offset_limit_radians;
            approach_value(&data->f68, 20.0f, frame_time * 6.0f);
            target_yaw_scratch =
                fast_subtract_rotations(fast_arc_tan(target_moby->pos.x - turret_moby->pos.x,
                                                     target_moby->pos.y - turret_moby->pos.y),
                                        data->f54);
            yaw_offset_limit_radians = data->f6C * 0.017453292f;
            if (yaw_offset_limit_radians < target_yaw_scratch) {
                target_yaw_scratch = yaw_offset_limit_radians;
            } else if (target_yaw_scratch < -yaw_offset_limit_radians) {
                target_yaw_scratch = -yaw_offset_limit_radians;
            }
            target_yaw_scratch = fast_add_rotations(target_yaw_scratch, data->f54);
            approach_angle(&turret_moby->rot.z,
                           target_yaw_scratch,
                           &data->f60,
                           frame_time_sq * 12.566371f,
                           frame_time_sq * 12.566371f,
                           frame_time * 12.566371f);
            FUN_L13_002b47a0_2b0e80(turret_moby);
            if (data->gun == 0 || data->gun->cls_id != 0x24 ||
                (unsigned char)data->gun->state == 0xFE ||
                (unsigned char)data->gun->state == 0xFD || (unsigned char)data->gun->state == 2) {
                if (turret_moby->b53 != 0) {
                    blend_moby_animation(turret_moby, 0, 0, random_int_between(7, 13));
                }
                data->f64 = 1.0f;
                turret_moby->state = 5;
            }
        }
        break;
    case BLARG_TURRET_STATE_DESTROY: {
        V4_2B5368 destruction_effect_vector;
        random_vector(&destruction_effect_vector.x, 0.5f, 1.0f);
        mark_moby_for_removal_cf(turret_moby, 0, -1);
        spawn_explosion(turret_moby,
                        &destruction_effect_vector,
                        &turret_moby->pos,
                        0.0f,
                        0.0f,
                        5,
                        2,
                        4,
                        2.0f,
                        1.0f,
                        9.0f,
                        -1,
                        1.0f,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        mark_moby_for_removal(turret_moby);
        return;
    }
    case BLARG_TURRET_STATE_DESTRUCTION_DELAY:
        approach_angle(&turret_moby->rot.z,
                       data->f54,
                       &data->f60,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 25.132742f);
        data->f64 -= 1.0f;
        if (data->f64 <= 0.0f) {
            turret_moby->state = 4;
        }
        break;
    }
    gun = data->gun;
    if (gun != 0 && gun->cls_id == 0x24 && (unsigned char)gun->state != 0xFE &&
        (unsigned char)gun->state != 0xFD) {
        qcopy(&gun->rot, &turret_moby->rot);
        *(u128 *)&gun->mat[0] = *(u128 *)&turret_moby->mat[0];
        *(u128 *)&gun->mat[1] = *(u128 *)&turret_moby->mat[1];
        *(u128 *)&gun->mat[2] = *(u128 *)&turret_moby->mat[2];
    } else {
        data->gun = 0;
    }
}

extern float blarg_turret_launch_displacement_scale_gp __asm__("D_L13_00161388")
    __attribute__((sda));

int D_L13_0016138C = 2;

void fire_blarg_turret_projectile(struct Moby *moby) __asm__("FUN_L13_002b47a0");

void fire_blarg_turret_projectile(struct Moby *moby) {
    float projectile_spawn_position[4];
    float horizontal_offset[4];
    char *blarg_turret_vars;
    float target_distance, x, projectile_yaw;
    blarg_turret_vars = (char *)moby->pvars;
    if (frame_count % D_L13_0016138C == 0) {
        moby_point_position(moby, 0, projectile_spawn_position);
        target_distance =
            vector_distance_xy(&moby->pos, *(char **)(blarg_turret_vars + 0x40) + 0x10);
        x = (20.0f - target_distance) / 20.0f * 5.0f + 2.5f;
        projectile_yaw = fast_add_rotations(random_float_between(-x, x) * DEG_TO_RAD, moby->rot.z);
        horizontal_offset[0] =
            fast_cos(projectile_yaw) * (blarg_turret_launch_displacement_scale_gp * frame_time);
        horizontal_offset[1] =
            fast_sin(projectile_yaw) * (blarg_turret_launch_displacement_scale_gp * frame_time);
        horizontal_offset[2] = 0;
        fast_vec_add(projectile_spawn_position, projectile_spawn_position, horizontal_offset);
        spawn_gemlik_projectile(*(float *)(blarg_turret_vars + 0x68),
                                projectile_spawn_position,
                                horizontal_offset,
                                moby);
    }
}

extern int select_moby_combat_target(void *, void *, float) __asm__("FUN_L00_0025fcb8");

void update_blarg_turret_target_and_state(struct Moby *moby) __asm__("FUN_L13_002b48d8");

void update_blarg_turret_target_and_state(struct Moby *moby) {
    char *turret_vars = (char *)moby->pvars;
    float distance;
    if (moby->state == 3)
        distance = 22.0f;
    else
        distance = 20.0f;
    select_moby_combat_target(moby, turret_vars, distance);
    if (*(int *)(turret_vars + 0x40) == 0) {
        *(int *)(turret_vars + 0x40) = ((int)hero.moby);
    }
    if (moby->command >= 4)
        moby->state = moby->command;
}

extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            int,
                            float,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

extern float approach_value(float *, float, float);

extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
