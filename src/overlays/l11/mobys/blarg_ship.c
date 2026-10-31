/* Level 11 moby class blarg_ship (class 0x527): circles, follows a path, strafes and fires; counting and activating listed ships. */
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/level_labels.h"

extern char *
spawn_moby_with_random_yaw(void *position, void *vector, int id) __asm__("FUN_L11_0030f5a8");
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_00310A70), where it is exact; names translated to the US level program. */

char *spawn_moby_with_random_yaw(void *position, void *vector, int id) {
    char *moby = create_moby(id);
    if (moby != 0) {
        char *spawned_moby_vars;
        float angle;
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        moby[0xBC] = 0;
        spawned_moby_vars = ((char *)MOBY(moby)->pvars);
        qcopy(moby + 0x10, position);
        qcopy(spawned_moby_vars + 0x10, vector);
        angle = random_angle_radians();
        *(int *)(moby + 0x40) = 0;
        MOBY(moby)->rot.z = angle;
        MOBY(moby)->rot.y = 0.7853982f;
        update_entity(moby);
        copy_hero_moby_field_38(moby);
    }
    return moby;
}

extern float frame_scale_inv_macro[] __asm__("D_0015ED68") MACRO_ADDR;

extern char *D_L11_0015FFD8;

#include "rnc/overlay/level_path.h"

extern void approach_angle(char *, char *, float, float, float, float) __asm__("FUN_L00_0025be00");

extern short *D_L11_001AC240[];
extern f32    advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern f32    approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");

extern char  D_00141C18[];
extern float D_L11_0015F580[] MACRO_ADDR;
extern void  transform_vector_by_basis(void *, void *, void *);

/* UpdateMoby 1319: a flying craft that circles, follows a path, strafes the player and fires. */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031ACA8), where it is exact; names translated to the US level program. */

extern char *spawn_blarg_ship_shot(
    int owner, float *dir, float *pos, float size, float len, float z) __asm__("FUN_L11_00308f48");
extern char  blarg_ship_positive_y_local_shot_offset[] __asm__("D_L11_001F14B0");
extern char  blarg_ship_negative_y_local_shot_offset[] __asm__("D_L11_001F14C0");
extern float D_L11_0015F580[];
extern float D_L11_001677C0_c[] __asm__("D_L11_001677C0");
extern float D_L11_001677D0[];
extern void  allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void  FUN_L00_0025f730(void *a, float x);
extern void  update_moby_trail_points(void *) __asm__("FUN_L11_00319670");
extern void  update_moby_hit_stats_and_state(void *) __asm__("FUN_L11_0031a1c0");
extern void  update_moby_path_transform(void *) __asm__("FUN_L11_0031a438");

extern short blarg_ship_shot_speed_gp __asm__("D_L11_001623D0") __attribute__((sda));
void         blarg_ship_update(struct Moby *moby) __asm__("FUN_L11_00319838");
extern f32   D_L11_001677D0[];
extern float approach_value(float *p, float target, float maxstep);
extern float vector_length_xy(void *);
extern int   scale_game_frames(int);

/* Moby class 1319 */
void blarg_ship_update(struct Moby *moby) {
    enum {
        BLARG_SHIP_INITIALIZE = 0,
        BLARG_SHIP_FOLLOW_PATH = 1,
        BLARG_SHIP_BURST_AND_WAIT_TO_REACTIVATE = 2,
        BLARG_SHIP_WAIT_TO_REACTIVATE = 3,
        BLARG_SHIP_INACTIVE = 4,
        BLARG_SHIP_MANEUVER_AND_FIRE = 5,
        BLARG_SHIP_BURST_AND_DEACTIVATE = 6
    };
    float scratch_vector[4];
    float target_position[4];
    float hero_position_delta[4];
    float basis_rotation_angles[4];
    float basis_row0[4];
    float basis_row1[4];
    float basis_row2[4];
    float direction_to_hero[4];
    float shot_displacement[4];
    float shot_velocity[4];
    float sA0[4];
    float sB0[4];
    float movement_distance_scratch;
    char *moby_vars = (char *)moby->pvars;
    float placement_angle;
    float distance_to_target;
    char *spawned_shot;

    update_moby_hit_stats_and_state(moby);
    switch (moby->state) {
    case BLARG_SHIP_INITIALIZE:
        moby->draw_dist = 0xFF;
        moby->update_dist = 0xFF;
        if (*(int *)(moby_vars + 0x88) == 0) {
            moby->state = 1;
            *(float *)(moby_vars + 0x6C) = randf_sym(0.3f, 0.5f);
            *(float *)(moby_vars + 0x70) = random_angle_radians();
            *(float *)(moby_vars + 0x74) =
                randf_sym(60.0f, 120.0f) * DEG_TO_RAD * frame_scale_inv_macro[1];
            *(float *)(moby_vars + 0x78) = random_float_between(8.0f, 12.0f);
        } else {
            moby->state = 4;
            moby->collision = 0;
            moby->flags |= 1;
        }
        break;
    case BLARG_SHIP_FOLLOW_PATH:
    case BLARG_SHIP_BURST_AND_WAIT_TO_REACTIVATE:
        update_moby_trail_points(moby);
        *(float *)(moby_vars + 0x68) += *(float *)(moby_vars + 0x6C);
        if (*(float *)(moby_vars + 0x68) > 1.0f) {
            *(float *)(moby_vars + 0x68) -= 1.0f;
            *(int *)(moby_vars + 0x64) =
                (*(int *)(moby_vars + 0x64) + 1) % l11_paths[*(int *)(moby_vars + 0x60)]->count;
        } else if (*(float *)(moby_vars + 0x68) < 0.0f) {
            *(float *)(moby_vars + 0x68) += 1.0f;
            *(int *)(moby_vars + 0x64) =
                (*(int *)(moby_vars + 0x64) + l11_paths[*(int *)(moby_vars + 0x60)]->count - 1) %
                l11_paths[*(int *)(moby_vars + 0x60)]->count;
        }
        fast_add_rotations(*(float *)(moby_vars + 0x70), *(float *)(moby_vars + 0x74));
        update_moby_path_transform(moby);
        if (moby->state == 2) {
            cart_burst(moby, &moby->pos, -1, 2.0f, 13.0f);
            if (random_integer_below(2)) {
                spawn_moby_with_random_yaw(&moby->pos, D_L11_0015F580, 0x4C4);
            } else {
                spawn_moby_with_random_yaw(&moby->pos, D_L11_0015F580, 0x4C2);
            }
            allocate_voice_for_target_entry(1, 0, (int)moby);
            *(unsigned short *)(D_00141C18) = 0xFFFF;
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
                *(int *)(moby_vars + 0x84) += 1;
                if (scale_game_frames(60) < *(int *)(moby_vars + 0x84)) {
                    moby->state = 1;
                    if (moby->prev_seq) {
                        blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                    }
                    moby->flags &= 0xFFFE;
                    moby->collision = moby->pclass->collision;
                }
            } else {
                *(int *)(moby_vars + 0x84) = 0;
            }
        } else {
            *(int *)(moby_vars + 0x84) = 0;
        }
        break;
    case BLARG_SHIP_INACTIVE:
        break;
    case BLARG_SHIP_MANEUVER_AND_FIRE:
        movement_distance_scratch = 0.0f;
        qcopy(basis_rotation_angles, D_L11_001677D0);
        basis_rotation_angles[0] = 0.0f;
        if (basis_rotation_angles[1] > 0.2617994f) {
            basis_rotation_angles[1] = 0.2617994f;
        } else if (basis_rotation_angles[1] < -0.2617994f) {
            basis_rotation_angles[1] = -0.2617994f;
        }
        euler_rotation_basis(basis_row0, basis_rotation_angles);
        fast_vec_sub(hero_position_delta, &hero.motion.pos, moby_vars + 0xB0);
        qcopy(moby_vars + 0xB0, &hero.motion.pos);
        fast_vec_add(&moby->pos, &moby->pos, hero_position_delta);
        placement_angle =
            fast_add_rotations(0.7853982f, (float)*(int *)(moby_vars + 0x88) * 1.5707964f);
        fast_vec_normalize(scratch_vector, basis_row1, fast_sin(placement_angle) * 8.0f);
        fast_vec_add(target_position, D_L11_001677C0_c, scratch_vector);
        fast_vec_normalize(scratch_vector, basis_row2, fast_cos(placement_angle) * 8.0f);
        fast_vec_add(target_position, target_position, scratch_vector);
        fast_vec_normalize(scratch_vector, basis_row0, 20.0f);
        fast_vec_add(target_position, target_position, scratch_vector);
        fast_vec_sub(scratch_vector, target_position, &moby->pos);
        distance_to_target = fast_vec_length(scratch_vector);
        if (*(char *)&hero.ship_flags & 2) {
            advance_accelerated_scalar(&movement_distance_scratch,
                                       (float *)(moby_vars + 0x6C),
                                       distance_to_target,
                                       frame_time_sq + frame_time_sq,
                                       frame_time_sq * 20.0f,
                                       frame_scale_inv_macro[1]);
        } else if (moby->command) {
            advance_accelerated_scalar(&movement_distance_scratch,
                                       (float *)(moby_vars + 0x6C),
                                       distance_to_target,
                                       frame_time_sq + frame_time_sq,
                                       frame_time_sq * 20.0f,
                                       frame_scale_inv_macro[1] * 15.0f);
        } else {
            advance_accelerated_scalar(&movement_distance_scratch,
                                       (float *)(moby_vars + 0x6C),
                                       distance_to_target,
                                       frame_time_sq + frame_time_sq,
                                       frame_time_sq * 20.0f,
                                       frame_scale_inv_macro[1] * 5.0f);
        }
        FUN_L00_0025f730(scratch_vector, *(float *)(moby_vars + 0x6C));
        fast_vec_add(&moby->pos.x, &moby->pos.x, scratch_vector);
        if (moby->command) {
            if (moby->visible) {
                *(int *)(moby_vars + 0x98) += 1;
            }
            if (fast_vec_dist(target_position, &moby->pos.x) < 2.0f) {
                moby->command = 0;
            } else if (scale_game_frames(120) < *(int *)(moby_vars + 0x98)) {
                moby->command = 0;
            }
        } else {
            float *hero_position = (float *)(&hero.motion.pos);
            float *yaw = &moby->rot.z;
            float *pitch = &moby->rot.y;
            float  steering_target_angle;
            fast_vec_sub(direction_to_hero, hero_position, &moby->pos.x);
            steering_target_angle = fast_arc_tan(direction_to_hero[0], direction_to_hero[1]);
            approach_angle(yaw,
                           (float *)(moby_vars + 0x8C),
                           steering_target_angle,
                           frame_time_sq * 12.566371f,
                           frame_time_sq * 12.566371f,
                           frame_scale_inv_macro[1] * 12.566371f);
            steering_target_angle =
                -fast_arc_tan(vector_length_xy(direction_to_hero), direction_to_hero[2]);
            approach_angle(pitch,
                           (float *)(moby_vars + 0x90),
                           steering_target_angle,
                           frame_time_sq * 12.566371f,
                           frame_time_sq * 12.566371f,
                           frame_scale_inv_macro[1] * 12.566371f);
            if (level_frame % 10 == 0) {
                allocate_voice_for_target_entry(2, 0, (int)moby);
                approach_value((float *)(moby_vars + 0x94), 0.0f, 0.25f);
                fast_vec_normalize(sB0, hero_position_delta, *(float *)(moby_vars + 0x94));
                fast_vec_add(sB0, sB0, hero_position);
                if (level_frame % 20 == 0) {
                    transform_vector_by_basis(
                        sA0, blarg_ship_positive_y_local_shot_offset, &moby->rot_row0);
                } else {
                    transform_vector_by_basis(
                        sA0, blarg_ship_negative_y_local_shot_offset, &moby->rot_row0);
                }
                fast_vec_add(sA0, sA0, &moby->pos.x);
                fast_vec_sub(shot_displacement, sB0, sA0);
                fast_vec_normalize(shot_velocity,
                                   shot_displacement,
                                   *(float *)&blarg_ship_shot_speed_gp * frame_scale_inv_macro[1]);
                fast_vec_add(shot_velocity, shot_velocity, hero_position_delta);
                spawned_shot =
                    spawn_blarg_ship_shot((int)moby, shot_velocity, sA0, 128.0f, -1.0f, 10.0f);
                if (spawned_shot) {
                    fast_vec_sub(shot_velocity, shot_velocity, hero_position_delta);
                    *(float *)(spawned_shot + 0x44) =
                        -fast_arc_tan(vector_length_xy(shot_velocity), shot_velocity[2]);
                    *(float *)(spawned_shot + 0x48) =
                        fast_arc_tan(shot_velocity[0], shot_velocity[1]);
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
        *(unsigned short *)(D_00141C18) = 0xFFFF;
        fast_vec_sub(scratch_vector, &hero.motion.pos, moby_vars + 0xB0);
        qcopy(moby_vars + 0xB0, &hero.motion.pos);
        fast_vec_add(&moby->pos, &moby->pos, scratch_vector);
        cart_burst(moby, &moby->pos, -1, 2.0f, 13.0f);
        if (random_integer_below(2)) {
            spawn_moby_with_random_yaw(&moby->pos, D_L11_0015F580, 0x4C4);
        } else {
            spawn_moby_with_random_yaw(&moby->pos, D_L11_0015F580, 0x4C2);
        }
        allocate_voice_for_target_entry(1, 0, (int)moby);
        moby->state = 4;
        moby->collision = 0;
        moby->flags |= 1;
        break;
    }
}
/* Counts the listed mobys of class 0x527 in state 5. */

short count_listed_blarg_ships_in_state_5(int moby_list_index) __asm__("FUN_L11_0031a758");

short count_listed_blarg_ships_in_state_5(int moby_list_index) {
    short *moby_list_entry = D_L11_001AC240[moby_list_index];
    short  found = 0;
    short  n = 0;
    char  *moby_array_base = D_L11_0015FFD8;
    do {
        unsigned char *moby =
            (unsigned char *)(((moby_list_entry[0] & 0x7FFF) << 8) + (int)moby_array_base);
        if (MOBY(moby)->oclass == 0x527 && moby[0x20] == 5) {
            found = ++n;
        }
    } while (*moby_list_entry++ >= 0);
    return found;
}

extern char D_L11_001677C0[];

char *activate_listed_blarg_ship(int list) __asm__("FUN_L11_0031a7d8");

char *activate_listed_blarg_ship(int list) {
    short *moby_list_entry = D_L11_001AC240[list];
    float  A[4], B[4], C[4], D[4], E[4], F[4];
    char  *selected_moby;
    char  *moby_vars;
    int    moby_byte_offset;

    do {
        moby_byte_offset = (*moby_list_entry & 0x7FFF) << 8;
        if (*(short *)(0xA6 + moby_byte_offset + D_L11_0015FFD8) != 0x527)
            continue;
        if (*(unsigned char *)(0x20 + moby_byte_offset + D_L11_0015FFD8) != 4)
            continue;
        if (random_integer_below(10))
            continue;
        selected_moby = D_L11_0015FFD8 + moby_byte_offset;
        moby_vars = ((char *)MOBY(selected_moby)->pvars);
        qcopy(C, D_L11_001677D0);
        C[0] = 0;
        if (C[1] > 0.17453292f)
            C[1] = 0.17453292f;
        else if (C[1] < -0.17453292f)
            C[1] = -0.17453292f;
        euler_rotation_basis(D, C);
        {
            float placement_angle =
                fast_add_rotations(0.7853982f, (float)*(int *)(moby_vars + 0x88) * 1.5707964f);
            fast_vec_normalize(A, E, fast_sin(placement_angle) * 12.0f);
            fast_vec_add(B, D_L11_001677C0, A);
            fast_vec_normalize(A, F, fast_cos(placement_angle) * 12.0f);
        }
        fast_vec_add(B, B, A);
        fast_vec_normalize(A, D, 6.0f);
        fast_vec_add(B, B, A);
        qcopy(moby_vars + 0xB0, D_0013E533 + 0xE9D);
        qcopy(selected_moby + 0x10, B);
        selected_moby[0x20] = 5;
        selected_moby[0xBC] = 1;
        *(float *)(moby_vars + 0x6C) = frame_scale_inv_macro[1] * 20.0f;
        *(float *)(moby_vars + 0x94) = 5.0f;
        *(int *)(moby_vars + 0x98) = 0;
        if (MOBY(selected_moby)->prev_seq)
            blend_moby_animation(selected_moby, 0, 0, scale_game_frames(10));
        MOBY(selected_moby)->flags &= 0xFFFE;
        MOBY(selected_moby)->collision = *(int *)(((char *)MOBY(selected_moby)->pclass) + 0x10);
        return selected_moby;
    } while (*moby_list_entry++ >= 0);
    return 0;
}
