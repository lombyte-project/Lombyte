/* Level 17 moby class blarg_ship: the fleet craft that circles, follows its path, strafes the player and fires, and its launch from a list. */
#include "types.h"
#include "rnc/math/vector.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "qcopy.h"
#include "rnc/overlay/l17/mobys/blarg_ship_globals.h"

/* UpdateMoby 1843: a flying fleet craft that circles, follows a path, strafes the player and fires. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F5C18), where it is exact; names translated to the US level program. */

extern char *spawn_blarg_ship_shot(
    int owner, float *dir, float *pos, float size, float len, float z) __asm__("FUN_L11_00308f48");
/* Shot launch offsets in the ship's frame; transform_vector_by_basis loads
   each as one quadword (lqc2). */
extern Vec4 blarg_ship_positive_y_launch_offset __asm__("D_L17_001DE110");
extern Vec4 blarg_ship_negative_y_launch_offset __asm__("D_L17_001DE120");
extern f32  advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern f32  approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern int  FUN_0022da68(int, int, int);
extern unsigned char *spawn_ship_pickup(char *position, int moby_class) __asm__("FUN_L13_002e1c50");
extern void           FUN_L00_0025f730(void *a, float x);
extern void           update_moby_trail_points(void *) __asm__("FUN_L11_00319670");
extern void           update_moby_hit_stats_and_state(void *) __asm__("FUN_L11_0031a1c0");
extern void           update_moby_path_transform(void *) __asm__("FUN_L11_0031a438");

float blarg_ship_firing_speed_gp __asm__("D_L17_001623E8") = 32.0f;
void  blarg_ship_update(struct Moby *moby) __asm__("FUN_L17_002f40d8");

/* Moby class 1843 */
void blarg_ship_update(struct Moby *moby) {
    enum {
        BLARG_SHIP_INITIALIZE = 0,
        BLARG_SHIP_FOLLOW_PATH = 1,
        BLARG_SHIP_BURST_AND_HIDE = 2,
        BLARG_SHIP_WAIT_TO_REACTIVATE = 3,
        BLARG_SHIP_INACTIVE = 4,
        BLARG_SHIP_TRACK_HERO_AND_FIRE = 5,
        BLARG_SHIP_BURST_AND_DEACTIVATE = 6
    };
    float scratch_vector[4];
    float movement_target_position[4];
    float hero_position_delta[4];
    float clamped_euler_angles[4];
    float rotation_basis_row0[4];
    float rotation_basis_row1[4];
    float rotation_basis_row2[4];
    float hero_relative_position[4];
    float firing_target_delta[4];
    float firing_direction[4];
    float sA0[4];
    float sB0[4];
    float movement_distance_accumulator;
    char *moby_pvars = (char *)moby->pvars;
    float offset_angle;
    float movement_target_distance;
    char *firing_moby;

    update_moby_hit_stats_and_state(moby);
    switch (moby->state) {
    case BLARG_SHIP_INITIALIZE:
        moby->draw_dist = 0xFF;
        moby->update_dist = 0xFF;
        if (*(int *)(moby_pvars + 0x88) == 0) {
            moby->state = 1;
            *(float *)(moby_pvars + 0x6C) = randf_sym(0.3f, 0.5f);
            *(float *)(moby_pvars + 0x70) = random_angle_radians();
            *(float *)(moby_pvars + 0x74) = randf_sym(60.0f, 120.0f) * DEG_TO_RAD * frame_time;
            *(float *)(moby_pvars + 0x78) = random_float_between(8.0f, 12.0f);
        } else {
            moby->state = 4;
            moby->collision = 0;
            moby->flags |= 1;
        }
        break;
    case BLARG_SHIP_FOLLOW_PATH:
    case BLARG_SHIP_BURST_AND_HIDE:
        update_moby_trail_points(moby);
        *(float *)(moby_pvars + 0x68) += *(float *)(moby_pvars + 0x6C);
        if (*(float *)(moby_pvars + 0x68) > 1.0f) {
            *(float *)(moby_pvars + 0x68) -= 1.0f;
            *(int *)(moby_pvars + 0x64) =
                (*(int *)(moby_pvars + 0x64) + 1) %
                **(int **)(D_L17_001B0DB0 + *(int *)(moby_pvars + 0x60) * 4);
        } else if (*(float *)(moby_pvars + 0x68) < 0.0f) {
            *(float *)(moby_pvars + 0x68) += 1.0f;
            *(int *)(moby_pvars + 0x64) =
                (*(int *)(moby_pvars + 0x64) +
                 **(int **)(D_L17_001B0DB0 + *(int *)(moby_pvars + 0x60) * 4) - 1) %
                **(int **)(D_L17_001B0DB0 + *(int *)(moby_pvars + 0x60) * 4);
        }
        fast_add_rotations(*(float *)(moby_pvars + 0x70), *(float *)(moby_pvars + 0x74));
        update_moby_path_transform(moby);
        if (moby->state == 2) {
            cart_burst(moby, &moby->pos, -1, 2.0f, 13.0f);
            if (random_integer_below(2)) {
                spawn_ship_pickup((char *)&moby->pos, 0xE0);
            } else {
                spawn_ship_pickup((char *)&moby->pos, 0xE4);
            }
            FUN_0022da68(1, 0, (int)moby);
            moby->state = 3;
            moby->collision = 0;
            moby->flags |= 1;
        } else if (moby->visible == 0) {
            if (fast_vec_dist(&moby->pos, &hero.motion.pos) < 32.0f) {
                moby->state = 3;
                moby->collision = 0;
                moby->flags |= 1;
            }
        }
        break;
    case BLARG_SHIP_WAIT_TO_REACTIVATE:
        qcopy(scratch_vector, &moby->pos);
        scratch_vector[3] = 3.0f;
        if (fast_bsphere_check((char *)&moby->pos, 512.0f) == -1) {
            if (fast_vec_dist(&moby->pos, &hero.motion.pos) > 32.0f) {
                *(int *)(moby_pvars + 0x84) += 1;
                if (scale_game_frames(60) < *(int *)(moby_pvars + 0x84)) {
                    moby->state = 1;
                    if (moby->prev_seq) {
                        blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                    }
                    moby->flags &= 0xFFFE;
                    moby->collision = moby->pclass->collision;
                }
            } else {
                *(int *)(moby_pvars + 0x84) = 0;
            }
        } else {
            *(int *)(moby_pvars + 0x84) = 0;
        }
        break;
    case BLARG_SHIP_INACTIVE:
        break;
    case BLARG_SHIP_TRACK_HERO_AND_FIRE:
        movement_distance_accumulator = 0.0f;
        qcopy(clamped_euler_angles, D_L17_001676D0);
        clamped_euler_angles[0] = 0.0f;
        if (clamped_euler_angles[1] > 0.2617994f) {
            clamped_euler_angles[1] = 0.2617994f;
        } else if (clamped_euler_angles[1] < -0.2617994f) {
            clamped_euler_angles[1] = -0.2617994f;
        }
        euler_rotation_basis(rotation_basis_row0, clamped_euler_angles);
        fast_vec_sub(hero_position_delta, &hero.motion.pos, moby_pvars + 0xB0);
        qcopy(moby_pvars + 0xB0, &hero.motion.pos);
        fast_vec_add(&moby->pos, &moby->pos, hero_position_delta);
        offset_angle =
            fast_add_rotations(0.7853982f, (float)*(int *)(moby_pvars + 0x88) * 1.5707964f);
        fast_vec_normalize(scratch_vector, rotation_basis_row1, fast_sin(offset_angle) * 8.0f);
        fast_vec_add(movement_target_position, D_L17_001676C0, scratch_vector);
        fast_vec_normalize(scratch_vector, rotation_basis_row2, fast_cos(offset_angle) * 8.0f);
        fast_vec_add(movement_target_position, movement_target_position, scratch_vector);
        fast_vec_normalize(scratch_vector, rotation_basis_row0, 20.0f);
        fast_vec_add(movement_target_position, movement_target_position, scratch_vector);
        fast_vec_sub(scratch_vector, movement_target_position, &moby->pos);
        movement_target_distance = fast_vec_length(scratch_vector);
        if (*(char *)&hero.ship_flags & 2) {
            advance_accelerated_scalar(&movement_distance_accumulator,
                                       (float *)(moby_pvars + 0x6C),
                                       movement_target_distance,
                                       frame_time_sq + frame_time_sq,
                                       frame_time_sq * 20.0f,
                                       frame_time);
        } else if (moby->command) {
            advance_accelerated_scalar(&movement_distance_accumulator,
                                       (float *)(moby_pvars + 0x6C),
                                       movement_target_distance,
                                       frame_time_sq + frame_time_sq,
                                       frame_time_sq * 20.0f,
                                       frame_time * 15.0f);
        } else {
            advance_accelerated_scalar(&movement_distance_accumulator,
                                       (float *)(moby_pvars + 0x6C),
                                       movement_target_distance,
                                       frame_time_sq + frame_time_sq,
                                       frame_time_sq * 20.0f,
                                       frame_time * 5.0f);
        }
        FUN_L00_0025f730(scratch_vector, *(float *)(moby_pvars + 0x6C));
        fast_vec_add(&moby->pos.x, &moby->pos.x, scratch_vector);
        if (moby->command) {
            if (moby->visible) {
                *(int *)(moby_pvars + 0x98) += 1;
            }
            if (fast_vec_dist(movement_target_position, &moby->pos.x) < 2.0f) {
                moby->command = 0;
            } else if (scale_game_frames(120) < *(int *)(moby_pvars + 0x98)) {
                moby->command = 0;
            }
        } else {
            float *hero_position = (float *)(&hero.motion.pos);
            float *yaw = &moby->rot.z;
            float *pitch = &moby->rot.y;
            float  target_rotation_angle;
            fast_vec_sub(hero_relative_position, hero_position, &moby->pos.x);
            target_rotation_angle =
                fast_arc_tan(hero_relative_position[0], hero_relative_position[1]);
            approach_angle(yaw,
                           (float *)(moby_pvars + 0x8C),
                           target_rotation_angle,
                           frame_time_sq * 12.566371f,
                           frame_time_sq * 12.566371f,
                           frame_time * 12.566371f);
            target_rotation_angle =
                -fast_arc_tan(vector_length_xy(hero_relative_position), hero_relative_position[2]);
            approach_angle(pitch,
                           (float *)(moby_pvars + 0x90),
                           target_rotation_angle,
                           frame_time_sq * 12.566371f,
                           frame_time_sq * 12.566371f,
                           frame_time * 12.566371f);
            if (D_L17_0015F5CC % 10 == 0) {
                FUN_0022da68(2, 0, (int)moby);
                approach_value((float *)(moby_pvars + 0x94), 0.0f, 0.25f);
                fast_vec_normalize(sB0, hero_position_delta, *(float *)(moby_pvars + 0x94));
                fast_vec_add(sB0, sB0, hero_position);
                if (D_L17_0015F5CC % 20 == 0) {
                    transform_vector_by_basis(
                        sA0, &blarg_ship_positive_y_launch_offset, &moby->rot_row0);
                } else {
                    transform_vector_by_basis(
                        sA0, &blarg_ship_negative_y_launch_offset, &moby->rot_row0);
                }
                fast_vec_add(sA0, sA0, &moby->pos.x);
                fast_vec_sub(firing_target_delta, sB0, sA0);
                fast_vec_normalize(
                    firing_direction, firing_target_delta, blarg_ship_firing_speed_gp * frame_time);
                fast_vec_add(firing_direction, firing_direction, hero_position_delta);
                firing_moby =
                    spawn_blarg_ship_shot((int)moby, firing_direction, sA0, 128.0f, -1.0f, 10.0f);
                if (firing_moby) {
                    fast_vec_sub(firing_direction, firing_direction, hero_position_delta);
                    MOBY(firing_moby)->rot.y =
                        -fast_arc_tan(vector_length_xy(firing_direction), firing_direction[2]);
                    MOBY(firing_moby)->rot.z =
                        fast_arc_tan(firing_direction[0], firing_direction[1]);
                }
            }
            if (moby->visible == 0) {
                moby->state = 4;
                moby->collision = 0;
                moby->flags |= 1;
            }
        }
        break;
    case BLARG_SHIP_BURST_AND_DEACTIVATE:
        fast_vec_sub(scratch_vector, &hero.motion.pos, moby_pvars + 0xB0);
        qcopy(moby_pvars + 0xB0, &hero.motion.pos);
        fast_vec_add(&moby->pos, &moby->pos, scratch_vector);
        cart_burst(moby, &moby->pos, -1, 2.0f, 13.0f);
        if (random_integer_below(2)) {
            spawn_ship_pickup((char *)&moby->pos, 0xE0);
        } else {
            spawn_ship_pickup((char *)&moby->pos, 0xE4);
        }
        FUN_0022da68(1, 0, (int)moby);
        moby->state = 4;
        moby->collision = 0;
        moby->flags |= 1;
        break;
    }
}

/* Finds the listed moby of type 0x733 in state 4 and launches it along a rotated vector. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F6AF8), where it is exact; names translated to the US level program. */

extern char D_0013E533[];

char *try_activate_blarg_ship_from_list(int list) __asm__("FUN_L17_002f4fb8");

char *try_activate_blarg_ship_from_list(int list) {
    short *moby_index_ptr = D_L17_001AC140[list];
    float  A[4], B[4], C[4], D[4], E[4], F[4];
    char  *selected_moby;
    char  *moby_pvars;
    int    moby_byte_offset;
    do {
        moby_byte_offset = (*moby_index_ptr & 0x7FFF) << 8;
        if (*(short *)(0xA6 + moby_byte_offset + D_L17_0015FFD8) != 0x733)
            continue;
        if (*(unsigned char *)(0x20 + moby_byte_offset + D_L17_0015FFD8) != 4)
            continue;
        if (random_integer_below(10))
            continue;
        selected_moby = D_L17_0015FFD8 + moby_byte_offset;
        moby_pvars = ((char *)MOBY(selected_moby)->pvars);
        qcopy(C, D_L17_001676D0);
        C[0] = 0;
        if (C[1] > 0.17453292f)
            C[1] = 0.17453292f;
        else if (C[1] < -0.17453292f)
            C[1] = -0.17453292f;
        euler_rotation_basis(D, C);
        {
            float offset_angle =
                fast_add_rotations(0.7853982f, (float)*(int *)(moby_pvars + 0x88) * 1.5707964f);
            fast_vec_normalize(A, E, fast_sin(offset_angle) * 12.0f);
            fast_vec_add(B, D_L17_001676C0, A);
            fast_vec_normalize(A, F, fast_cos(offset_angle) * 12.0f);
        }
        fast_vec_add(B, B, A);
        fast_vec_normalize(A, D, 6.0f);
        fast_vec_add(B, B, A);
        qcopy(moby_pvars + 0xB0, D_0013E533 + 0xE9D);
        qcopy(selected_moby + 0x10, B);
        selected_moby[0x20] = 5;
        selected_moby[0xBC] = 1;
        *(float *)(moby_pvars + 0x6C) = frame_time * 20.0f;
        *(float *)(moby_pvars + 0x94) = 5.0f;
        if (MOBY(selected_moby)->prev_seq)
            blend_moby_animation(selected_moby, 0, 0, scale_game_frames(10));
        MOBY(selected_moby)->flags &= 0xFFFE;
        MOBY(selected_moby)->collision = *(int *)(((char *)MOBY(selected_moby)->pclass) + 0x10);
        return selected_moby;
    } while (*moby_index_ptr++ >= 0);
    return 0;
}
