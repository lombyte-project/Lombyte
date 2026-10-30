/* Level 05 moby class extermibot. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/build_symmetric_adjacency_masks.h"
#include "qcopy.h"

extern int   scale_game_frames(int) __asm__("FUN_001f96f8");
extern void  blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern int   random_integer_below(int) __asm__("FUN_00213260");
extern void  fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float fast_cos(float);
extern float fast_sin(float);
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float D_0015ED6C;
extern float D_0015ED70;
float        absolute_float(float input) __asm__("func_001F99C0");
extern float FUN_L00_0025be00(float *, float *, float, float, float, float);
extern int   FUN_L00_00258b50(void *, void *, void *, void *, float);
extern void  FUN_L01_0026d930(char *p);
/* Extermibot update: wakes from its dormant pose, walks its path, closes in on the target and strikes it, bursts into sparks between strikes, and dies on a hit. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003033A0), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

extern char  D_L05_0015F580[] MACRO_ADDR;
extern char  D_L05_001671C0[];
extern f32   approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern float FUN_L04_001f1978(float);
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_vec_dist(void *, void *);
extern int   D_L05_0015FFD8;
extern int   D_L05_0015F5C4;
extern int   step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");
extern int   FUN_L00_00261b48(int, void *, void *, void *);
extern int   is_point_inside_clip_volume(void *arg0, int arg1) __asm__("func_00214720");
extern s32   follow_path(void *, s32, s32, void *, s32, void *, f32) __asm__("FUN_L01_00276fe8");
extern s32
spawn_spark_c(void *, void *, void *, s32, s32, s32, s32, s32, s32) __asm__("func_00218888");
extern int   D_L05_00161A84 __attribute__((sda));
extern float D_L05_00161A98 __attribute__((sda));

#include "rnc/overlay/l05/mobys/extermibot_data/extermibot_update_2.h"

extern float extermibot_spark_paired_frame_count_min_gp __asm__("D_L05_00161AB0")
    __attribute__((sda));
extern float D_L05_00161AB4 __attribute__((sda));

#include "rnc/overlay/l05/mobys/extermibot_data/extermibot_update.h"

extern void FUN_L00_0024f7c8(void *, int, void *);
extern void FUN_L00_00257d78(float *, float, float);
extern void
spawn_nearby_effect(void *, int, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
extern void FUN_L00_0025a120(void *);
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void
resolve_point_path_segment_xy_clearance(int, void *, void *, float) __asm__("FUN_L00_00261d78");
extern void
FUN_L00_00263fd8_c(void *, int, void *, void *, int, int, float, void *, void *, void *) __asm__("FUN_L00_00263fd8");
extern void  mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
f32          compute_interpolated_record_value(void *anim) __asm__("FUN_0020c9e0");
void         FUN_L05_00303390_c(Moby *m) __asm__("FUN_L05_00303390");
void         FUN_L05_00303b08_c(struct Moby *m, s32 node, f32 side) __asm__("FUN_L05_00303b08");
extern float D_0013F3D0[];
extern char  extermibot_strike_flags_selector[] __asm__("D_001404A8");

void extermibot_update(unsigned char *m) __asm__("FUN_L05_00301f48");

void extermibot_update(unsigned char *moby) {
    enum {
        EXTERMIBOT_INITIALIZE = 0,
        EXTERMIBOT_IDLE_AND_CHECK_TARGET_PATH = 1,
        EXTERMIBOT_WANDER_NEAR_START = 2,
        EXTERMIBOT_FOLLOW_TARGET = 4,
        EXTERMIBOT_WAIT_FOR_TARGET_IN_STRIKE_RANGE = 5,
        EXTERMIBOT_APPROACH_TARGET_DIRECTLY = 6,
        EXTERMIBOT_AIM_BEFORE_STRIKE = 7,
        EXTERMIBOT_STRIKE_TARGET = 8,
        EXTERMIBOT_RECOVER_AFTER_STRIKE = 9,
        EXTERMIBOT_RETURN_TO_START = 10,
        EXTERMIBOT_NONLETHAL_KNOCKBACK = 11,
        EXTERMIBOT_WAIT_FOR_ANIMATION_BEFORE_IDLE = 13,
        EXTERMIBOT_DEATH_KNOCKBACK = 14,
        EXTERMIBOT_WAIT_FOR_ACTIVATION_TRIGGER = 15
    };
    char          *moby_data = ((char *)MOBY(moby)->pvars);
    char          *target_moby;
    float          path_goal_or_facing_vector[4];
    float          out[4];
    float          hit[4];
    float          spark_spawn_position[4];
    int            i;
    int            j;
    float         *moby_yaw_ptr;
    float          yaw;
    unsigned char *moby_position;
    extern float   fast_vec_dist(void *, void *);

    manipulator_set_scale(moby_data + 0x170, 2.5f);
    if (D_L05_0015F5C4 == 2 && ((MOBY(moby)->flags ^ 1) & 1)) {
        *(int *)(moby_data + 0x2DC) = 1;
        MOBY(moby)->flags |= 0x41;
    } else if (*(int *)(moby_data + 0x2DC) != 0 && D_L05_0015F5C4 == 0) {
        *(int *)(moby_data + 0x2DC) = 0;
        MOBY(moby)->flags &= 0xFFBE;
    }
    if (moby[0x31] != 0 &&
        fast_vec_dist((float *)&MOBY(moby)->pos, (float *)D_L05_001671C0) < 28.0f) {
        FUN_L00_0025a120(moby);
        moby[0x7F] = 0x16;
    }
    FUN_L05_00303390_c((Moby *)moby);
    target_moby = *(char **)(moby_data + 0x160);
    MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * (D_L05_00161A98);
    switch (moby[0x20]) {
    case EXTERMIBOT_INITIALIZE:
        MOBY(moby)->flags |= 0x1000;
        qcopy(moby_data + 0x270, moby + 0x10);
        *(float *)(moby_data + 0x20) = 2.0f;
        moby_data[0x29] = 0;
        *(int *)(moby_data + 0x2B0) = 0;
        moby_data[0x28] = 1;
        *(float *)(moby_data + 0x30) = 1.0f;
        *(short *)(moby_data + 0x24) = 2;
        moby_data[0x58] = truncate_float_to_s32(14.0f);
        moby_data[0x5A] = truncate_float_to_s32(11.2f);
        FUN_L01_0026d930(moby_data + 0xD0);
        {
            float movement_scale = D_L05_00161A98;
            *(float *)(moby_data + 0xDC) = 2.0f;
            *(float *)(moby_data + 0xD8) = 2.0f;
            {
                int   quantized_movement_scale = truncate_float_to_s32(movement_scale * 409.6f);
                float initial_movement_speed = D_0015ED6C * 6.0f;
                *(int *)(moby_data + 0xD0) = quantized_movement_scale;
                *(float *)(moby_data + 0xF4) = initial_movement_speed;
            }
        }
        *(float *)(moby_data + 0x2D8) = random_angle_radians();
        if (random_integer_below(2) != 0) {
            MOBY(moby)->flags |= 0x8000;
        } else {
            MOBY(moby)->flags &= 0x7FFF;
        }
        if (*(int *)(moby_data + 0x2B4) != 0) {
            moby[0x20] = 0xC;
            moby[0x31] = 0;
            MOBY(moby)->collision = 0;
            MOBY(moby)->flags &= 0xEFFF;
            MOBY(moby)->pos.z -= 20.0f;
            MOBY(moby)->draw_dist = 0;
        } else if (*(long *)(moby_data + 0x2B8) != -1) {
            MOBY(moby)->flags &= 0xEFFF;
            moby[0x20] = 0xF;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, 0);
        } else {
            moby[0x20] = 1;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, 0);
        }
        build_symmetric_adjacency_masks((int *)(moby_data + 0x2C0), 1, *(int *)(moby_data + 0x2C4));
        break;
    case EXTERMIBOT_IDLE_AND_CHECK_TARGET_PATH:
        if (*(int *)(moby_data + 0x164) != 2 && random_integer_below(9) == 0) {
            if (follow_path(moby_data + 0x2C0,
                            1,
                            *(int *)(moby_data + 0x2C4),
                            moby + 0x10,
                            (s32)(target_moby + 0x10),
                            path_goal_or_facing_vector,
                            (D_L05_00161A98) * 0.4f) != 0) {
                if (random_integer_below(0x100) & 1) {
                    *(float *)(moby_data + 0x298) = -*(float *)(moby_data + 0x298);
                }
                *(float *)(moby_data + 0x294) = *(float *)(moby_data + 0x298) * 0.017453292f;
                *(int *)(moby_data + 0x2AC) =
                    scale_game_frames((int)random_float_between(30.0f, 90.0f));
                moby[0x20] = 4;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            } else {
                *(int *)(moby_data + 0x164) = 2;
            }
        }
        if (*(int *)(moby_data + 0x164) == 2 && *(int *)(moby_data + 0x2B0) != 0) {
            moby[0x20] = 6;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        } else if (moby[0x70] & 2) {
            if (moby[0x31] != 0 && (random_integer_below(0xFF) & 1)) {
                FUN_L00_00257d78((float *)(moby_data + 0x280), 0.0f, 4.0f);
                *(float *)(moby_data + 0x288) = 0.0f;
                fast_vec_add(moby_data + 0x280, moby_data + 0x280, moby_data + 0x270);
                *(short *)(moby_data + 0x2D2) = 0;
                moby[0xBC] = moby[0x20];
                moby[0x20] = 2;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            } else if (moby[0x53] != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
        }
        break;
    case EXTERMIBOT_WANDER_NEAR_START:
        moby_yaw_ptr = (float *)&MOBY(moby)->rot.z;
        moby_position = moby + 0x10;
        yaw = fast_arc_tan(*(float *)(moby_data + 0x280) - MOBY(moby)->pos.x,
                           *(float *)(moby_data + 0x284) - MOBY(moby)->pos.y);
        FUN_L00_0025be00(moby_yaw_ptr,
                         (float *)(moby_data + 0x290),
                         yaw,
                         D_0015ED70 * 6.2831855f,
                         D_0015ED70 * 6.2831855f,
                         D_0015ED6C * 6.2831855f);
        if (fast_difference_between_rotations(
                MOBY(moby)->rot.z,
                fast_arc_tan(*(float *)(moby_data + 0x280) - MOBY(moby)->pos.x,
                             *(float *)(moby_data + 0x284) - MOBY(moby)->pos.y)) < 1.5707964f) {
            path_goal_or_facing_vector[0] = fast_cos(MOBY(moby)->rot.z) * 2.0f;
            path_goal_or_facing_vector[1] = fast_sin(MOBY(moby)->rot.z) * 2.0f;
            path_goal_or_facing_vector[2] = 0.0f;
            FUN_L00_00258b50(moby, moby_data + 0xD0, path_goal_or_facing_vector, out, 1.0f);
            resolve_point_path_segment_xy_clearance(
                *(int *)(moby_data + 0x2C0), moby_position, moby_position, (D_L05_00161A98) * 0.4f);
        }
        (*(short *)(moby_data + 0x2D2))++;
        if ((vector_distance_xy(moby_position, moby_data + 0x280) < 1.0f &&
             *(short *)(moby_data + 0x2D2) > scale_game_frames(0x3C)) ||
            *(short *)(moby_data + 0x2D2) > scale_game_frames(0xB4)) {
            moby[0x20] = moby[0xBC];
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(20));
        }
        break;
    case EXTERMIBOT_APPROACH_TARGET_DIRECTLY: {
        int near;
        moby_yaw_ptr = (float *)&MOBY(moby)->rot.z;
        moby_position = moby + 0x10;
        approach_value((float *)(moby_data + 0xF4), D_0015ED6C * 6.0f, D_0015ED70 * 6.0f);
        yaw = fast_arc_tan(MOBY(target_moby)->pos.x - MOBY(moby)->pos.x,
                           MOBY(target_moby)->pos.y - MOBY(moby)->pos.y);
        FUN_L00_0025be00(moby_yaw_ptr,
                         (float *)(moby_data + 0x290),
                         yaw,
                         D_0015ED70 * 6.2831855f,
                         D_0015ED70 * 6.2831855f,
                         D_0015ED6C * 6.2831855f);
        if (fast_difference_between_rotations(
                MOBY(moby)->rot.z,
                fast_arc_tan(MOBY(target_moby)->pos.x - MOBY(moby)->pos.x,
                             MOBY(target_moby)->pos.y - MOBY(moby)->pos.y)) < 1.5707964f) {
            path_goal_or_facing_vector[0] = fast_cos(MOBY(moby)->rot.z) * 2.0f;
            path_goal_or_facing_vector[1] = fast_sin(MOBY(moby)->rot.z) * 2.0f;
            path_goal_or_facing_vector[2] = 0.0f;
            FUN_L00_00258b50(moby, moby_data + 0xD0, path_goal_or_facing_vector, out, 1.0f);
            resolve_point_path_segment_xy_clearance(
                *(int *)(moby_data + 0x2C0), moby_position, moby_position, (D_L05_00161A98) * 0.4f);
        }
        near = 0;
        if (FUN_L00_00261b48(*(int *)(moby_data + 0x2C0), moby_position, target_moby + 0x10, hit) !=
                0 &&
            fast_vec_dist((float *)moby_position, hit) < 1.5f) {
            near = 1;
        }
        if (near) {
            moby[0x20] = 5;
            if (moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(20));
        } else if (*(int *)(moby_data + 0x2B0) != 0) {
            if (vector_distance_xy(moby_position, target_moby + 0x10) < 3.0f) {
                moby[0x20] = 7;
                *(short *)(moby_data + 0x2D0) =
                    scale_game_frames(extermibot_pre_strike_delay_frames_gp);
                if (moby[0x53] != 2)
                    blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            }
        } else {
            moby[0x20] = 10;
        }
        break;
    }
    case EXTERMIBOT_WAIT_FOR_TARGET_IN_STRIKE_RANGE:
        if (*(int *)(moby_data + 0x2B0) == 0) {
            *(float *)(moby_data + 0xF4) = 0.0f;
            moby[0x20] = 10;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
        } else if (vector_distance_xy(moby + 0x10, target_moby + 0x10) < 3.0f) {
            moby[0x20] = 7;
            *(short *)(moby_data + 0x2D0) =
                scale_game_frames(extermibot_pre_strike_delay_frames_gp);
            if (moby[0x53] != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
        }
        break;
    case EXTERMIBOT_WAIT_FOR_ACTIVATION_TRIGGER:
        if ((*(int *)(moby_data + 0x2B8) != -1 &&
             ((unsigned char *)*(int *)&D_L05_0015FFD8)[*(int *)(moby_data + 0x2B8) * 256 + 0xBC] !=
                 0) ||
            (*(int *)(moby_data + 0x2BC) != -1 &&
             is_point_inside_clip_volume(((char *)D_0013F3D0), *(int *)(moby_data + 0x2BC)) != 0)) {
            MOBY(moby)->draw_dist = 0x40;
            MOBY(moby)->flags |= 0x1000;
            moby[0x20] = 1;
        } else if (moby[0x70] & 2) {
            if (random_integer_below(0xFF) & 1) {
                FUN_L00_00257d78((float *)(moby_data + 0x280), 0.0f, 4.0f);
                *(float *)(moby_data + 0x288) = 0.0f;
                fast_vec_add(moby_data + 0x280, moby_data + 0x280, moby_data + 0x270);
                *(short *)(moby_data + 0x2D2) = scale_game_frames(0x5A);
                moby[0xBC] = moby[0x20];
                moby[0x20] = 2;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            } else if (moby[0x53] != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
        }
        break;
    case EXTERMIBOT_FOLLOW_TARGET: {
        float target_distance_xy = vector_distance_xy(moby + 0x10, target_moby + 0x10);
        FUN_L05_00303b08_c(
            (struct Moby *)moby, (s32)(target_moby + 0x10), *(float *)(moby_data + 0x294));
        if (target_distance_xy < 3.0f &&
            fast_difference_between_rotations(
                MOBY(moby)->rot.z,
                fast_arc_tan(MOBY(target_moby)->pos.x - MOBY(moby)->pos.x,
                             MOBY(target_moby)->pos.y - MOBY(moby)->pos.y)) < 0.7853982f) {
            moby[0x20] = 7;
            *(short *)(moby_data + 0x2D0) =
                scale_game_frames(extermibot_pre_strike_delay_frames_gp);
            if (moby[0x53] != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
        }
        if (*(int *)(moby_data + 0x164) == 2) {
            moby[0x20] = 10;
        }
        break;
    }
    case EXTERMIBOT_AIM_BEFORE_STRIKE:
        moby_yaw_ptr = (float *)&MOBY(moby)->rot.z;
        yaw = fast_arc_tan(MOBY(target_moby)->pos.x - MOBY(moby)->pos.x,
                           MOBY(target_moby)->pos.y - MOBY(moby)->pos.y);
        FUN_L00_0025be00(moby_yaw_ptr,
                         (float *)(moby_data + 0x290),
                         yaw,
                         D_0015ED70 * 12.566371f,
                         D_0015ED70 * 12.566371f,
                         D_0015ED6C * 25.132742f);
        if ((moby[0x70] & 2) && moby[0x53] == 2) {
            blend_moby_animation(moby, 7, 0, scale_game_frames(5));
        }
        if (fast_dec_timer_s16(moby_data + 0x2D0) != 0) {
            float a;
            moby[0x20] = 8;
            *(short *)(moby_data + 0x2D0) = 0;
            if (moby[0x53] != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(1));
            a = fast_add_rotations(MOBY(moby)->rot.z, 0.0f);
            path_goal_or_facing_vector[0] =
                fast_cos(a) * (extermibot_spark_facing_xy_scale_gp * D_0015ED6C);
            path_goal_or_facing_vector[1] =
                fast_sin(a) * (extermibot_spark_facing_xy_scale_gp * D_0015ED6C);
            path_goal_or_facing_vector[2] = 0.0f;
            path_goal_or_facing_vector[3] = extermibot_spark_facing_vector_w_gp;
            out[2] = 0.0f;
            out[3] = extermibot_spark_random_vector_w_gp;
            FUN_L00_0024f7c8(moby, 0, hit);
            for (i = 0x1D; i >= 0; i--) {
                int c1, c2;
                qcopy(spark_spawn_position, hit);
                spark_spawn_position[0] += random_float_between(-0.065f, 0.065f);
                spark_spawn_position[1] += random_float_between(-0.065f, 0.065f);
                spark_spawn_position[2] += random_float_between(-0.065f, 0.065f);
                out[0] = random_float_between(-D_0015ED6C, D_0015ED6C);
                out[1] = random_float_between(-D_0015ED6C, D_0015ED6C);
                out[2] = random_float_between(-(extermibot_spark_random_z_scale_gp * D_0015ED6C),
                                              extermibot_spark_random_z_scale_gp * D_0015ED6C);
                c1 = truncate_float_to_s32(FUN_L04_001f1978(
                    random_float_between(extermibot_spark_lifetime_min_frames_gp,
                                         extermibot_spark_lifetime_min_frames_gp * 2.0f)));
                c2 = truncate_float_to_s32(FUN_L04_001f1978(
                    random_float_between(extermibot_spark_paired_frame_count_min_gp,
                                         extermibot_spark_paired_frame_count_min_gp * 1.2f)));
                spawn_spark_c(spark_spawn_position,
                              path_goal_or_facing_vector,
                              out,
                              extermibot_primary_spark_initial_color_gp,
                              extermibot_primary_spark_paired_color_gp,
                              c1,
                              c2,
                              truncate_float_to_s32(FUN_L04_001f1978(
                                  random_float_between(D_L05_00161AB4, D_L05_00161AB4 * 1.2f))),
                              -1);
            }
            path_goal_or_facing_vector[3] = extermibot_spark_facing_vector_w_gp * 0.1f;
            out[3] = extermibot_spark_random_vector_w_gp * 0.1f;
            for (j = 0x1D; j >= 0; j--) {
                int c1, c2;
                qcopy(spark_spawn_position, hit);
                spark_spawn_position[0] += random_float_between(-0.065f, 0.065f);
                spark_spawn_position[1] += random_float_between(-0.065f, 0.065f);
                spark_spawn_position[2] += random_float_between(-0.065f, 0.065f);
                out[0] = random_float_between(-D_0015ED6C, D_0015ED6C);
                out[1] = random_float_between(-D_0015ED6C, D_0015ED6C);
                out[2] = random_float_between(-(extermibot_spark_random_z_scale_gp * D_0015ED6C),
                                              extermibot_spark_random_z_scale_gp * D_0015ED6C);
                c1 = truncate_float_to_s32(FUN_L04_001f1978(
                    random_float_between(extermibot_spark_lifetime_min_frames_gp,
                                         extermibot_spark_lifetime_min_frames_gp * 2.0f)));
                c2 = truncate_float_to_s32(FUN_L04_001f1978(
                    random_float_between(extermibot_spark_paired_frame_count_min_gp,
                                         extermibot_spark_paired_frame_count_min_gp * 1.2f)));
                spawn_spark_c(spark_spawn_position,
                              path_goal_or_facing_vector,
                              out,
                              extermibot_secondary_spark_initial_color_gp,
                              extermibot_secondary_spark_paired_color_gp,
                              c1,
                              c2,
                              truncate_float_to_s32(FUN_L04_001f1978(
                                  random_float_between(D_L05_00161AB4, D_L05_00161AB4 * 1.2f))),
                              -1);
            }
        }
        break;
    case EXTERMIBOT_STRIKE_TARGET: {
        int   fl;
        float a = fast_add_rotations(MOBY(moby)->rot.z, 0.0f);
        if (compute_interpolated_record_value(moby) < 1.5f &&
            fast_difference_between_rotations(
                a,
                fast_arc_tan(MOBY(target_moby)->pos.x - MOBY(moby)->pos.x,
                             MOBY(target_moby)->pos.y - MOBY(moby)->pos.y)) < 0.2617994f &&
            vector_distance_xy(moby + 0x10, target_moby + 0x10) < 3.0f &&
            absolute_float(MOBY(target_moby)->pos.z - MOBY(moby)->pos.z) < 2.0f) {
            path_goal_or_facing_vector[0] = fast_cos(MOBY(moby)->rot.z);
            path_goal_or_facing_vector[1] = fast_sin(MOBY(moby)->rot.z);
            path_goal_or_facing_vector[2] = 0.0f;
            path_goal_or_facing_vector[3] = 5627.9248f;
            qcopy(out, target_moby + 0x10);
            out[2] += 0.75f;
            spawn_nearby_effect(
                target_moby,
                (int)moby,
                *(int *)(((char *)&extermibot_strike_flags_selector)) != 6 ? 0x10001 : 0x10000,
                out,
                path_goal_or_facing_vector,
                1.0f);
        }
        if (moby[0x70] & 2) {
            moby[0x20] = 9;
            if (moby[0x53] != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
        }
        break;
    }
    case EXTERMIBOT_RECOVER_AFTER_STRIKE:
        if (moby[0x70] & 2) {
            if (vector_distance_xy(moby + 0x10, target_moby + 0x10) < 3.0f &&
                fast_difference_between_rotations(
                    MOBY(moby)->rot.z,
                    fast_arc_tan(MOBY(target_moby)->pos.x - MOBY(moby)->pos.x,
                                 MOBY(target_moby)->pos.y - MOBY(moby)->pos.y)) < 0.2617994f) {
                moby[0x20] = 7;
                *(short *)(moby_data + 0x2D0) =
                    scale_game_frames(extermibot_pre_strike_delay_frames_gp);
                if (moby[0x53] != 7)
                    blend_moby_animation(moby, 7, 0, scale_game_frames(5));
            } else {
                *(float *)(moby_data + 0xF4) = 0.0f;
                moby[0x20] = 4;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        }
        break;
    case EXTERMIBOT_RETURN_TO_START:
        FUN_L05_00303b08_c((struct Moby *)moby, (s32)(moby_data + 0x270), 0.0f);
        if (vector_distance_xy(moby + 0x10, moby_data + 0x270) < 1.0f) {
            moby[0x20] = 1;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(3));
        } else if (*(int *)(moby_data + 0x164) != 2) {
            moby[0x20] = 4;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(3));
        } else if (*(int *)(moby_data + 0x2B0) != 0) {
            moby[0x20] = 6;
        }
        break;
    case EXTERMIBOT_NONLETHAL_KNOCKBACK:
        if (step_moby_motion(moby, moby_data + 0x70) & 0x60) {
            if (follow_path(moby_data + 0x2C0,
                            1,
                            *(int *)(moby_data + 0x2C4),
                            moby + 0x10,
                            (s32)(moby_data + 0x270),
                            path_goal_or_facing_vector,
                            (D_L05_00161A98) * 0.4f) != 0) {
                moby[0x20] = 4;
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                break;
            }
            cart_burst(moby, moby + 0x10, -1, 0.5f, 13.0f);
            mark_moby_for_removal_c2(moby);
            return;
        }
        if (MOBY(moby)->pos.z < 2.0f) {
            mark_moby_for_removal_c2(moby);
            return;
        }
        break;
    case EXTERMIBOT_WAIT_FOR_ANIMATION_BEFORE_IDLE:
        if (moby[0x70] & 2) {
            moby[0x20] = 1;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        }
        break;
    case EXTERMIBOT_DEATH_KNOCKBACK:
        if (step_moby_motion(moby, moby_data + 0x70) & 0x60) {
            cart_burst(moby, moby + 0x10, -1, 0.5f, 13.0f);
            FUN_L00_00263fd8_c(moby,
                               0x6DA,
                               moby + 0x10,
                               moby + 0x40,
                               0,
                               0,
                               0.0f,
                               D_L05_0015F580,
                               D_L05_0015F580,
                               D_L05_0015F580);
            FUN_L00_00263fd8_c(moby,
                               0x6DB,
                               moby + 0x10,
                               moby + 0x40,
                               0,
                               0,
                               0.0f,
                               D_L05_0015F580,
                               D_L05_0015F580,
                               D_L05_0015F580);
            FUN_L00_00263fd8_c(moby,
                               0x6DC,
                               moby + 0x10,
                               moby + 0x40,
                               0,
                               0,
                               0.0f,
                               D_L05_0015F580,
                               D_L05_0015F580,
                               D_L05_0015F580);
            mark_moby_for_removal_c2(moby);
            return;
        }
        if (MOBY(moby)->pos.z < 0.0f) {
            mark_moby_for_removal_c2(moby);
            return;
        }
        break;
    }
    D_L05_00161A84 = *(volatile int *)0x10000000;
}

extern float scale_time(float) __asm__("FUN_001f96b0");
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

/* Level 5 moby update (a flying enemy): takes hits through func_L00_0025B4D0 and reacts per hit kind, then
   follows its path, retargets the closest moby and turns its head toward the hero. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003047E8), where it is exact; names translated to the US level program. */

typedef int u128_3047E8 __attribute__((mode(TI)));

typedef float Vec_3047E8[4] __attribute__((aligned(16)));

typedef struct Hit_3047E8 {
    Vec_3047E8  at;
    u128_3047E8 dir;
    Moby       *who;
} Hit_3047E8;

typedef struct Path_3047E8 {
    int  n;
    char p0[0xC];
    int  pts[1];
} Path_3047E8;

typedef struct Wing {
    char          pad0[0x20];
    float         hp;
    char          pad1[0x14];
    int           f38;
    char          pad2[0x24];
    short         f60[3];
    char          pad3[0x1];
    unsigned char b67;
    char          pad4[0x8];
    char          f70[16];
    float         f80;
    char          pad5[0x4];
    float         f88;
    float         f8C;
    int           f90;
    int           f94;
    float         f98;
    char          pad6[0x11];
    unsigned char bAD;
    char          pad7[0x12];
    float         fC0;
    float         fC4;
    char          pad8[0x58];
    char          f120[8];
    float         f128;
    char          pad9[0x34];
    struct Moby  *target;
    int           f164;
    char          pad10[0x8];
    char          f170[16];
    char          pad11[0x54];
    float         f1D4;
    float         f1D8;
    char          pad12[0x14];
    char          f1F0[16];
    char          pad13[0x58];
    float         f258;
    char          pad14[0x14];
    char          f270[16];
    char          pad15[0x1C];
    float         f29C;
    float         f2A0;
    char          pad16[0xC];
    int           f2B0;
    char          pad17[0x14];
    int           f2C8;
    int           f2CC;
    char          pad18[0x4];
    int           f2D4;
    float         f2D8;
} Wing;

int D_L05_00161A74 = 0x80004000;
int D_L05_00161A78 = 0x80008000;

#include "rnc/overlay/l05/mobys/extermibot_data/handle_extermibot_hits_and_update_targeting_2.h"

extern float D_L05_00161A98_gp __asm__("D_L05_00161A98") __attribute__((sda));

#include "rnc/overlay/l05/mobys/extermibot_data/handle_extermibot_hits_and_update_targeting.h"

extern Hit_3047E8 *func_L00_0025B478_3047E8(void *, int, int) __asm__("FUN_L00_0025a420");
extern int         collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int resolve_moby_hit_response(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void trooper_knockback_arc(void *, float *, float *, float *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void stop_animation_l08(void *, int, int) __asm__("FUN_L00_00257470");
extern void trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, short *) __asm__("FUN_L00_0025d538");
extern int
activate_effect_l08(float, void *, void *, int, int, int *, int) __asm__("FUN_L00_0025ff38");
extern float func_001F9B88_3047E8(float) __asm__("FUN_001f99c0");
extern Moby *func_L05_003053D8_3047E8(void *, int) __asm__("FUN_L05_00303f80");
extern float func_001F9FA8_3047E8(float) __asm__("FUN_001f9de0");
extern void  func_001F9BF0_3047E8(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  update_joint_animation(void *, void *, int, float, float) __asm__("FUN_L00_002628d8");
extern char  D_0013F420[];
extern s32   D_001413D0_lui[] __asm__("D_001413D0") NOT_SDA;

void handle_extermibot_hits_and_update_targeting(Moby *moby) __asm__("FUN_L05_00303390");

void handle_extermibot_hits_and_update_targeting(Moby *moby) {
    enum {
        EXTERMIBOT_HIT_NO_RESPONSE = 0,
        EXTERMIBOT_HIT_LETHAL = 1,
        EXTERMIBOT_HIT_DEATH_KNOCKBACK = 2,
        EXTERMIBOT_HIT_NONLETHAL_KNOCKBACK = 3,
        EXTERMIBOT_HIT_KNOCKBACK_AND_FLASH = 4,
        EXTERMIBOT_HIT_NONLETHAL_KNOCKBACK_AND_FLASH = 5,
        EXTERMIBOT_HIT_START_NONLETHAL_KNOCKBACK = 6,
        EXTERMIBOT_HIT_START_NONLETHAL_KNOCKBACK_AND_FLASH = 7,
        EXTERMIBOT_HIT_START_KNOCKBACK_WITH_FLASH = 8,
        EXTERMIBOT_HIT_FLASH = 9,
        EXTERMIBOT_HIT_FLASH_ONLY = 10,
        EXTERMIBOT_HIT_NO_REACTION = 11
    };
    Vec_3047E8  working_vector;
    Vec_3047E8  hit_test_endpoint;
    int         collision_hit_status;
    float       dmg;
    float       nonlethal_knockback_heading;
    float       death_knockback_heading;
    Wing       *moby_data;
    Hit_3047E8 *hit_record;
    int         hit_response;
    int         k;

    moby_data = *(struct Wing **)&moby->pvars;
    fast_dec_timer(&moby_data->f2D4);
    dmg = 0.0f;
    hit_record = func_L00_0025B478_3047E8(moby, 0x330000, 0);
    if (hit_record != 0) {
        qcopy(working_vector, ((float *)&moby->pos));
        working_vector[2] = working_vector[2] + 0.5f;
        if (hit_record->who != 0) {
            if (hit_record->who->oclass == 0x47) {
                qcopy(hit_test_endpoint, D_0013F420);
            } else if ((*(struct Moby **)&hit_record->who->pclass != 0 &&
                        *(short *)((u8 *)((struct Moby *)hit_record->who->pclass) + 0x46) == 5) ||
                       hit_record->who->oclass == 0xBA) {
                qcopy(hit_test_endpoint, working_vector);
            } else {
                qcopy(hit_test_endpoint, ((float *)&hit_record->who->pos));
            }
        } else {
            qcopy(hit_test_endpoint, hit_record->at);
        }
        if (fast_vec_dist(working_vector, hit_test_endpoint) < 24.0f &&
            collision_line(working_vector, hit_test_endpoint, 2, moby, 0) != 0) {
            hit_record = 0;
        }
    }
    hit_response = resolve_moby_hit_response(
        moby, hit_record, &moby_data->hp, 0, &collision_hit_status, &dmg, 0, 4);
    if (collision_hit_status != 1 && moby->state != 14) {
        moby_data->hp = moby_data->hp - dmg;
        if (moby_data->hp <= 0.0f) {
            hit_response = 1;
        }
        k = truncate_float_to_s32(D_L05_00161A98_gp * 409.6f);
        moby_data->f98 = D_L05_00161A98_gp * 0.4f;
        moby_data->f80 = extermibot_knockback_gravity_gp * D_0015ED70;
        moby_data->f90 = k;
        moby_data->f94 = 9;
        moby_data->bAD = 0;
        switch (hit_response) {
        case EXTERMIBOT_HIT_NO_RESPONSE:
        case EXTERMIBOT_HIT_NO_REACTION:
            break;
        case EXTERMIBOT_HIT_FLASH:
        case EXTERMIBOT_HIT_FLASH_ONLY:
            moby_data->b67 = 0xFA;
            break;
        case EXTERMIBOT_HIT_NONLETHAL_KNOCKBACK:
        case EXTERMIBOT_HIT_KNOCKBACK_AND_FLASH:
        case EXTERMIBOT_HIT_NONLETHAL_KNOCKBACK_AND_FLASH:
        case EXTERMIBOT_HIT_START_NONLETHAL_KNOCKBACK:
        case EXTERMIBOT_HIT_START_NONLETHAL_KNOCKBACK_AND_FLASH:
        case EXTERMIBOT_HIT_START_KNOCKBACK_WITH_FLASH:
            moby_data->fC0 = 5.0f;
            moby_data->f88 = extermibot_nonlethal_knockback_xy_scale_gp * D_0015ED6C;
            moby_data->f8C = extermibot_nonlethal_knockback_z_scale_gp * D_0015ED6C;
            moby_data->fC4 = 10.0f;
            nonlethal_knockback_heading = fast_arc_tan(moby->pos.x - hit_record->who->pos.x,
                                                       moby->pos.y - hit_record->who->pos.y);
            *(u128_3047E8 *)working_vector = hit_record->dir;
            trooper_knockback_arc(
                working_vector, &nonlethal_knockback_heading, &moby_data->f88, &moby_data->f8C);
            trooper_start_knockback(moby, moby_data->f70, nonlethal_knockback_heading, 5, 1, 0);
            moby->state = 11;
            moby_data->b67 = 0x78;
            break;
        case EXTERMIBOT_HIT_LETHAL:
        case EXTERMIBOT_HIT_DEATH_KNOCKBACK:
            moby->flags &= 0xEFFF;
            moby_data->f80 = extermibot_death_knockback_gravity_gp * D_0015ED70;
            moby_data->f88 = extermibot_death_knockback_xy_scale_gp * D_0015ED6C;
            moby_data->f8C = extermibot_death_knockback_z_scale_gp * D_0015ED6C;
            moby_data->fC0 = 5.5f;
            moby_data->fC4 = 11.0f;
            death_knockback_heading = fast_arc_tan(moby->pos.x - hit_record->who->pos.x,
                                                   moby->pos.y - hit_record->who->pos.y);
            *(u128_3047E8 *)working_vector = hit_record->dir;
            trooper_knockback_arc(
                working_vector, &death_knockback_heading, &moby_data->f88, &moby_data->f8C);
            trooper_start_knockback(moby, moby_data->f70, death_knockback_heading, 6, 1, 0);
            moby->state = 14;
            moby_data->b67 = 0xF0;
            if (hit_record->who != 0 && *(struct Moby **)&hit_record->who->pclass != 0 &&
                *(short *)((u8 *)((struct Moby *)hit_record->who->pclass) + 0x46) == 5) {
                stop_animation_l08(moby, 0x800, -1);
            } else {
                stop_animation_l08(moby, 0, -1);
            }
            break;
        }
        trooper_anim_update(moby, moby_data->f60);
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, moby_data->f60);
    if (moby_data->f38 != 0) {
        moby_data->f2B0 = truncate_float_to_s32(scale_time(random_float_between(180.0f, 240.0f)));
        moby_data->f38 = 0;
    }
    fast_dec_timer(&moby_data->f2B0);
    if (moby_data->f2B0 != 0) {
        moby_data->f2A0 = moby_data->f29C + 20.0f;
    } else {
        moby_data->f2A0 = moby_data->f29C;
    }
    if (activate_effect_l08(moby_data->f2A0,
                            moby,
                            moby_data->f120,
                            0,
                            0,
                            ((Path_3047E8 *)l05_paths[moby_data->f2CC])->pts,
                            ((Path_3047E8 *)l05_paths[moby_data->f2CC])->n) != 2) {
        if (moby_data->f2A0 < vector_distance_xy(moby_data->f270, moby_data->f120) ||
            3.0f < func_001F9B88_3047E8(moby->pos.z - moby_data->f128)) {
            moby_data->f164 = 2;
        }
    }
    if (moby_data->f2C8 >= 0) {
        Moby *candidate_target = func_L05_003053D8_3047E8(moby, moby_data->f2C8);
        if (candidate_target != 0) {
            if (moby_data->f164 == 2 ||
                vector_distance_xy(((float *)&moby->pos), ((float *)&candidate_target->pos)) <
                    vector_distance_xy(((float *)&moby->pos), ((float *)&moby_data->target->pos))) {
                moby_data->target = candidate_target;
                moby_data->f164 = 1;
            }
        }
    }
    if (moby_data->target == 0) {
        moby_data->target = (*(Moby **)D_001413D0_lui);
    }
    moby_data->f2D8 = fast_add_rotations(moby_data->f2D8, D_0015ED6C * 9.424778f);
    moby->glow_color = fast_tween_color(
        D_L05_00161A74, D_L05_00161A78, (func_001F9FA8_3047E8(moby_data->f2D8) + 1.0f) * 0.5f);
    k = moby->state;
    if (k < 10) {
        if (k >= 4 && moby_data->target == (*(Moby **)D_001413D0_lui)) {
            float yaw, pitch;
            func_001F9BF0_3047E8(
                working_vector, ((float *)&moby_data->target->pos), ((float *)&moby->pos));
            yaw = fast_subtract_rotations(fast_arc_tan(working_vector[0], working_vector[1]),
                                          moby->rot.z);
            pitch = -fast_arc_tan(vector_length_xy(working_vector), working_vector[2]);
            if (moby->flags & 0x8000) {
                yaw = -yaw;
            }
            if (1.5707964f < yaw) {
                yaw = 1.5707964f;
            } else if (yaw < -1.5707964f) {
                yaw = -1.5707964f;
            }
            if (0.5235988f < pitch) {
                pitch = 0.5235988f;
            } else if (pitch < -0.5235988f) {
                pitch = -0.5235988f;
            }
            moby_data->f1D4 = pitch;
            moby_data->f1D8 = yaw * 0.7f;
            moby_data->f258 = yaw * 0.3f;
        }
    }
    update_joint_animation(moby, moby_data->f170, 1, frame_scale_sq * 0.04f, frame_scale_sq * 0.3f);
    update_joint_animation(moby, moby_data->f1F0, 2, frame_scale_sq * 0.04f, frame_scale_sq * 0.3f);
}

extern int frame_number __asm__("D_L05_0015F5CC");

/* Pvars of a path-walking moby (only the fields this walker touches). */
typedef struct {
    u8    pad0[0xD0];
    u8    move[0x24]; /* 0xD0: ground-move state */
    f32   speed;      /* 0xF4 */
    u8    padF8[0x164 - 0xF8];
    s32   mode; /* 0x164: 2 once the path is finished */
    u8    pad168[0x280 - 0x168];
    Vec4f goal;    /* 0x280: current path target */
    f32   yaw_vel; /* 0x290 */
    u8    pad294[0x2C0 - 0x294];
    s32   path;     /* 0x2C0 */
    s32   path_pos; /* 0x2C4 */
    u8    pad2C8[0x2D4 - 0x2C8];
    s32   stuck_timer; /* 0x2D4 */
} PathWalkerVars;

extern f32 D_L05_00161A98_gp __asm__("D_L05_00161A98") __attribute__((sda));
extern f32 fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern f32
approach_angle(f32 *angle, f32 target, f32 *vel, f32, f32, f32) __asm__("FUN_L00_0025be00");

extern s32
test_extermibot_offset_path_clearance(char *moby, float *goal, float s) __asm__("FUN_L05_00303e50");
extern f32 FUN_L05_00304058_u(s32, void *, void *, f32) __asm__("FUN_L05_00304058");

/* Walks m toward its next path node, steering `side` radians off the direct heading while
   FUN_L05_00303e50 reports the way blocked; slows near the goal and arms the stuck timer
   when it drifts off the path. */
void move_extermibot_along_path(struct Moby *moby, s32 node, f32 side) __asm__("FUN_L05_00303b08");

void move_extermibot_along_path(struct Moby *moby, s32 node, f32 side) {
    PathWalkerVars *walker_vars = (PathWalkerVars *)moby->pvars;
    Vec4f           facing_direction, out, goal;
    f32             offset_heading;
    f32             distance_to_goal;

    if ((((s32)moby >> 8) & 3) == (frame_number & 3)) {
        if (follow_path(&walker_vars->path,
                        1,
                        walker_vars->path_pos,
                        &moby->pos,
                        node,
                        &walker_vars->goal,
                        D_L05_00161A98_gp * 0.4f) == 0)
            walker_vars->mode = 2;
    }
    qcopy(&goal, &walker_vars->goal);
    distance_to_goal = vector_distance_xy(&moby->pos, &goal);
    if (distance_to_goal > 5.0f && side != 0.0f &&
        test_extermibot_offset_path_clearance(
            (char *)moby, (float *)&goal, D_L05_00161A98_gp * 0.4f)) {
        offset_heading =
            fast_add_rotations(fast_arc_tan(goal.x - moby->pos.x, goal.y - moby->pos.y), side);
        approach_angle(&moby->rot.z,
                       offset_heading,
                       &walker_vars->yaw_vel,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 25.132742f);
    } else {
        approach_angle(&moby->rot.z,
                       fast_arc_tan(goal.x - moby->pos.x, goal.y - moby->pos.y),
                       &walker_vars->yaw_vel,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 25.132742f);
    }
    if (fast_difference_between_rotations(
            moby->rot.z, fast_arc_tan(goal.x - moby->pos.x, goal.y - moby->pos.y)) < 1.5707964f &&
        distance_to_goal > 3.0f)
        approach_value(&walker_vars->speed, frame_time * 6.0f, frame_time_sq * 6.0f);
    else
        approach_value(&walker_vars->speed, frame_time, frame_time_sq * 6.0f);
    facing_direction.x = fast_cos(moby->rot.z) * 2.0f;
    facing_direction.y = fast_sin(moby->rot.z) * 2.0f;
    facing_direction.z = 0.0f;
    FUN_L00_00258b50(moby, walker_vars->move, &facing_direction, &out, 1.0f);
    if (walker_vars->stuck_timer == 0) {
        if (FUN_L05_00304058_u(
                walker_vars->path, &moby->pos, &moby->pos, D_L05_00161A98_gp * 0.4f) > 2.0f)
            walker_vars->stuck_timer = scale_game_frames(5);
    }
}

extern float FUN_001f9dc8_c(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_c(float) __asm__("FUN_001f9de0");
extern void  FUN_001f9a10_c(float *, float *, float *) __asm__("FUN_001f9a10");
extern void  FUN_001f9a28_c(float *, float *, float *) __asm__("FUN_001f9a28");
extern void  FUN_001f9bf8_c(float *, float *, float) __asm__("FUN_001f9bf8");
extern int   test_path_segment_strict_crossing(int, float *, float *) __asm__("FUN_L00_00261968");

int test_extermibot_offset_path_clearance(char *moby, float *goal, float probe_offset_length) {
    float probe_position[4];
    float offset_probe_position[4];
    char *moby_data = ((char *)MOBY(moby)->pvars);
    float probe_heading;
    probe_heading = fast_arc_tan(goal[0] - MOBY(moby)->pos.x, goal[1] - MOBY(moby)->pos.y);
    probe_heading = fast_add_rotations(probe_heading, *(float *)(moby_data + 0x294) * 1.5f);
    probe_position[0] =
        FUN_001f9dc8_c(probe_heading) * fast_vec_dist(goal, (float *)&MOBY(moby)->pos);
    probe_position[1] =
        FUN_001f9de0_c(probe_heading) * fast_vec_dist(goal, (float *)&MOBY(moby)->pos);
    *(int *)&probe_position[2] = 0;
    FUN_001f9a10_c(probe_position, probe_position, (float *)&MOBY(moby)->pos);
    FUN_001f9a28_c(offset_probe_position, probe_position, goal);
    FUN_001f9bf8_c(offset_probe_position, offset_probe_position, probe_offset_length);
    FUN_001f9a10_c(offset_probe_position, offset_probe_position, (float *)&MOBY(moby)->pos);
    return test_path_segment_strict_crossing(
               *(int *)(moby_data + 0x2C0), probe_position, offset_probe_position) == 0;
}

extern struct Moby *D_L05_0015FFD8_7920 __asm__("D_L05_0015FFD8");

extern short *D_L05_001AC040_03f80[] __asm__("D_L05_001ABCC0");
extern short  D_L05_00160098_03f80 __asm__("D_L05_0015FFD8");
char *find_nearest_extermibot_group_target(char *self, int group_index) __asm__("FUN_L05_00303f80");

char *find_nearest_extermibot_group_target(char *self, int group_index) {
    short *list = D_L05_001AC040_03f80[group_index];
    float  best = 120.0f;
    char  *nearest_moby = 0;
    if (list == 0) {
        return 0;
    }
    {
        do {
            int i = *(unsigned short *)list & 0x7FFF;
            if (D_L05_0015FFD8_7920[i].flags & 0x1000) {
                if (D_L05_0015FFD8_7920[i].state < 0x7F) {
                    float candidate_distance =
                        fast_vec_dist(&D_L05_0015FFD8_7920[i].pos, self + 0x10);
                    if (candidate_distance < best) {
                        best = candidate_distance;
                        nearest_moby = *(char **)&D_L05_00160098_03f80 + (i << 8);
                    }
                }
            }
        } while (*list++ >= 0);
    }
    return nearest_moby;
}
/* Pushes a point clear of a path and returns the nearest wall distance, like
 * FUN_L00_00261d78. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003054B0), where it is exact; names translated to the US level program. */

typedef int Quad_3054B0 __attribute__((mode(TI)));

typedef union {
    Quad_3054B0 q;
    f32         f[4];
    s32         i[4];
} PathVec;

typedef struct {
    s32     count;
    u8      pad[12];
    PathVec points[1];
} SegmentPath;

extern f32  fast_vec_dot(void *, void *) __asm__("func_001F9AB0");
extern void func_001F9BD8_3054B0(PathVec *, PathVec *, PathVec *) __asm__("FUN_001f9a10");
extern void func_001F9BF0_3054B0(PathVec *, PathVec *, PathVec *) __asm__("FUN_001f9a28");
extern void func_001F9CA0_3054B0(PathVec *, PathVec *, PathVec *) __asm__("FUN_001f9ad8");
extern f32  func_001F9CB8_3054B0(PathVec *) __asm__("FUN_001f9af0");
extern void func_L00_001FF4B0_3054B0(PathVec *, PathVec *, f32) __asm__("FUN_001f9bf8");

f32 push_point_from_path(s32   path_index,
                         void *input,
                         void *output,
                         f32   distance) __asm__("FUN_L05_00304058");

f32 push_point_from_path(s32 path_index, void *input, void *output, f32 distance) {
    PathVec      *point = input;
    PathVec      *out = output;
    PathVec       difference;
    PathVec       scaled_difference;
    PathVec       working;
    PathVec       segment;
    PathVec       scaled_segment;
    PathVec       cross;
    PathVec       scaled_cross;
    SegmentPath **entry;
    s32           i;
    s32           found;
    f32           best = 512.0f;

    found = 0;
    qcopy_nc(&working, point);
    for (i = 0; i < ((SegmentPath *)l05_paths[path_index])->count - 1; i++) {
        entry = ((SegmentPath **)&l05_paths[path_index]);
        if ((*entry)->points[i].f[3] == 0.0f && (*entry)->points[i + 1].f[3] == 0.0f)
            continue;
        func_001F9BF0_3054B0(&difference, &working, &(*entry)->points[i]);
        difference.i[2] = 0;
        func_001F9BF0_3054B0(&segment, &(*entry)->points[i + 1], &(*entry)->points[i]);
        segment.i[2] = 0;
        func_L00_001FF4B0_3054B0(&scaled_segment, &segment, 1.0f);
        func_001F9CA0_3054B0(&cross, &difference, &scaled_segment);
        {
            f32 wall_distance = absolute_float(cross.f[2]);
            if (wall_distance < best)
                best = wall_distance;
            if (distance < wall_distance)
                continue;
        }
        {
            f32 segment_length = func_001F9CB8_3054B0(&segment);
            f32 projection = fast_vec_dot(&difference, &scaled_segment);
            if (segment_length < projection || projection < 0.0f) {
                if (!(func_001F9CB8_3054B0(&difference) < distance))
                    continue;
                found = 1;
                func_L00_001FF4B0_3054B0(&scaled_difference, &difference, distance);
                func_001F9BD8_3054B0(&working, &scaled_difference, &(*entry)->points[i]);
                working.f[2] = point->f[2];
            } else {
                found = 1;
                func_L00_001FF4B0_3054B0(&scaled_cross, &scaled_segment, projection);
                func_001F9BF0_3054B0(&scaled_difference, &difference, &scaled_cross);
                func_L00_001FF4B0_3054B0(&scaled_difference, &scaled_difference, distance);
                func_001F9BD8_3054B0(&scaled_difference, &scaled_difference, &scaled_cross);
                func_001F9BD8_3054B0(&working, &scaled_difference, &(*entry)->points[i]);
                working.f[2] = point->f[2];
            }
        }
    }
    if (found)
        qcopy(out, &working);
    return best;
}
