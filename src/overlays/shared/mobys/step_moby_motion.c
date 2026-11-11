/* Moby motion: steps a moby's motion, transformed deltas, shape point particles and the trooper knockback start. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/mobys/get_moby_target_info.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/overlay/shared/props/grow_and_fade_out.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/shared/mobys/knockback_motion.h"

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025D288), where it is exact; names translated to the US level program. */

/* Returns component d of c - a transformed by the matrix transpose_rotation_matrix builds from b. */
float compute_transformed_delta_component(void *subtrahend_vector,
                                          void *rotation_matrix_source,
                                          void *minuend_vector,
                                          int   component_index) __asm__("FUN_L00_0025c230");

float compute_transformed_delta_component(void *subtrahend_vector,
                                          void *rotation_matrix_source,
                                          void *minuend_vector,

                                          int component_index) {
    float  delta[4], transformed_delta[4], transposed_rotation_matrix[16];
    float *transformed_delta_ptr = transformed_delta;
    fast_vec_sub(delta, minuend_vector, subtrahend_vector);
    transpose_rotation_matrix(transposed_rotation_matrix, rotation_matrix_source);
    transform_vector(transformed_delta_ptr, delta, transposed_rotation_matrix);
    return transformed_delta_ptr[component_index];
}

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025D308), where it is exact; names translated to the US level program. */

typedef struct {
    int a[4];
} Vq __attribute__((aligned(16)));

/* Sets a to b plus c (normalised) scaled by s times the dot of c and b. */
void subtract_homogeneous_scaled_vector_xyz(void *destination_vector,
                                            void *minuend_vector,
                                            void *direction_vector,
                                            float projection_scale) __asm__("FUN_L00_0025c2b0");

void subtract_homogeneous_scaled_vector_xyz(void *a,
                                            void *b,
                                            void *c,
                                            float s) __asm__("FUN_L00_0025c2b0");
void subtract_homogeneous_scaled_vector_xyz(void *destination_vector,
                                            void *minuend_vector,
                                            void *direction_vector,

                                            float projection_scale) {
    Vq scaled_vector, normalized_vector;
    fast_vec_normalize(&normalized_vector, direction_vector, 1.0f);
    fast_vec_scale(&scaled_vector,
                   &normalized_vector,
                   fast_vec_dot(&normalized_vector, minuend_vector) * projection_scale);
    fast_vec_sub(destination_vector, minuend_vector, &scaled_vector);
}

/* Spawns one particle per point found by func_L00_0025BCF8, at a random offset about A1. */
/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025D3F0), where it is exact; names translated to the US level program. */

typedef struct {
    float v[4];
} V4 __attribute__((aligned(16)));

extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int generate_random_points_from_shape_entries(void *, int, void *, void *, float) __asm__(
    "FUN_L00_0025aca0");
extern void spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");

void spawn_particles_at_shape_points(void *source_object,
                                     void *base_vector,
                                     void *point_source_mask) __asm__("FUN_L00_0025c398");

void spawn_particles_at_shape_points(void *source_object,
                                     void *base_vector,

                                     void *point_source_mask) {
    V4    generated_points[20];
    float randomized_vector[4];
    int   point_count, i;

    point_count = generate_random_points_from_shape_entries(
        source_object, 20, generated_points, point_source_mask, 14.0f);
    if (point_count == 0) {
        return;
    }
    for (i = 0; i < point_count; i++) {
        float          y = random_angle_radians();
        float          z = random_angle_radians();
        unsigned char *flag = item_text_variant;
        int            randomized_frame_count, randomized_record_value;

        build_spherical_offset(randomized_vector, frame_time * 1.8f, y, z);
        fast_vec_add(randomized_vector, randomized_vector, base_vector);
        randomized_vector[2] += frame_time * 0.9f;
        if (flag[0x10]) {
            randomized_frame_count = scale_game_frames(random_int_between(20, 35));
            randomized_record_value = random_int_between(40, 60);
            spawn_effect(&generated_points[i],
                         randomized_vector,
                         0x7000FFA0,
                         0xFF00,
                         randomized_frame_count,
                         30,
                         randomized_record_value,
                         1);
        } else {
            randomized_frame_count = scale_game_frames(random_int_between(20, 35));
            randomized_record_value = random_int_between(40, 60);
            spawn_effect(&generated_points[i],
                         randomized_vector,
                         0x7000A0FF,
                         0xFF,
                         randomized_frame_count,
                         30,
                         randomized_record_value,
                         1);
        }
    }
}

void blend_moby_animation_ex(void *, int, int, int, int) __asm__("FUN_002130d8");
void trooper_start_knockback(
    u8 *a, struct KnockbackMotion *b, int c, int d, int e, float f) __asm__("FUN_L00_0025c558");
void trooper_start_knockback(u8                     *moby,
                             struct KnockbackMotion *knockback_state,
                             int                     animation_index,
                             int                     animation_timing_divisor,
                             int                     animation_frame_index,

                             float knockback_angle) {
    enum {
        MOTION_RISING = 0
    };
    float *moby_vars_record;
    knockback_state->velocity[0] = fast_cos(knockback_angle) * knockback_state->speed;
    knockback_state->velocity[1] = fast_sin(knockback_angle) * knockback_state->speed;
    knockback_state->velocity[2] = knockback_state->vertical_speed;
    knockback_state->velocity[3] = 1.0f;
    knockback_state->turn_target = fast_add_rotations(knockback_angle, 3.1415927f);
    knockback_state->turn_velocity = 0.0f;
    knockback_state->timer = scale_game_frames(300);
    knockback_state->phase = 0;
    knockback_state->active = 1;
    moby_vars_record = get_moby_target_info(moby);
    if (moby_vars_record && *moby_vars_record == 0.0f) {
        knockback_state->flags |= 0x20;
        MOBY(moby)->collision = 0;
    }
    if (knockback_state->flags & 2)
        blend_moby_animation_ex(
            moby, animation_index, animation_frame_index, animation_timing_divisor, 1);
    else
        blend_moby_animation(
            moby, animation_index, animation_frame_index, animation_timing_divisor);
}

#include "rnc/math/vector.h"

#include "qcopy.h"

/* Steps moby MOBY's motion from state ST and returns the flag bits it set. */
/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025D6F0), where it is exact; names translated to the US level program. */

#include "rnc/overlay/shared/world/collision.h"

extern float approach_value(float *p, float target, float maxstep);
extern int  *D_L00_00177F00[];
extern Vec4  D_94C8_60 __asm__("D_L00_00173E60"); /* no foreign declaration */
extern int   coll_sphere() __asm__("FUN_L00_001f0d60");
extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int is_moby_in_class_range_500_to_540(void *) __asm__("FUN_L00_0025e3b8");
void       FUN_L00_00258278(unsigned char *m, float *ptr, float t, float b, float c, float limit);
void       FUN_L00_00259710(void *a);
void       spawn_nearby_effect(char *, float, int, int, void *, void *) __asm__("FUN_L00_00259bc8");
void spawn_particles_at_shape_points(void *a0, void *a1, void *a2) __asm__("FUN_L00_0025c398");
extern Vec4  D_94C8_60 __asm__("D_L00_00173E60");
extern float func_L00_0025A748_f(void *) __asm__("FUN_L00_00259710");
extern int   collision_line(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern int   func_L00_001F10E0_l(void *, float, int, void *) __asm__("FUN_L00_001f0d60");
extern int   func_L00_001F2BE8_d6f0(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");

int step_moby_motion(char *moby, struct KnockbackMotion *st) __asm__("FUN_L00_0025c698");

int step_moby_motion(char *moby, struct KnockbackMotion *motion_state) {
    enum {
        MOTION_RISING = 0,
        MOTION_FALLING = 1,
        MOTION_GROUNDED = 2
    };
    float eye[4];
    float height_probe_position[4];
    float collision_probe_position[4];
    float predicted_query_position[4];
    float surface_normal[4];
    float normal_response_vector[4];
    float horizontal_motion_magnitude, larger_quadratic_root, smaller_quadratic_root, f6c;

    Vec4 *collision_result_position;
    char *collision_metadata;
    int   motion_outcome_flags = 0;

    float sc, ground_reference_height, landing_prediction_divisor, f12, motion_or_record_value, r,
        rr, one;
    int result_count, i;

    motion_state->velocity[2] = motion_state->velocity[2] - motion_state->gravity;
    if (motion_state->particle_mode == 1) {
        if (motion_state->particle_count != 0) {
            spawn_particles_at_shape_points(
                moby, motion_state->velocity, (void *)(int)motion_state->particle_count);
        }
    }

    horizontal_motion_magnitude = vector_length_xy(motion_state->velocity);
    motion_or_record_value = (motion_state->phase == 2) ? 0.0f : motion_state->target_speed;
    approach_value(
        &horizontal_motion_magnitude, motion_or_record_value, motion_state->speed_approach_step);
    set_vector_length_xy(
        motion_state->velocity, motion_state->velocity, horizontal_motion_magnitude);
    if (motion_state->vertical_speed < motion_state->velocity[2]) {
        motion_state->velocity[2] = motion_state->vertical_speed;
    }

    qcopy(height_probe_position, &MOBY(moby)->pos);
    height_probe_position[2] = height_probe_position[2] + motion_state->collision_z_offset;
    ground_reference_height = probe_ground_height(height_probe_position, 0, 0.5f);
    if (collision_hit_material() == 0) {
        if (motion_state->ground_bounces == 0.0f &&
            ground_reference_height <= height_probe_position[2] &&
            height_probe_position[2] + motion_state->velocity[2] < ground_reference_height) {
            motion_state->ground_bounces = 1.0f;
            height_probe_position[2] = ground_reference_height;
            spawn_burst_moby(height_probe_position, 3.0f);
        }
        ground_reference_height = func_L00_0025A748_f(&MOBY(moby)->pos);
    } else {
        if ((motion_state->flags & 0x10) || motion_state->phase == 2) {
            fast_vec_add(collision_probe_position, &MOBY(moby)->pos, motion_state->velocity);
            if (func_L00_0025A748_f(collision_probe_position) < ground_reference_height - 0.25f) {
                set_vector_length_xy(motion_state->velocity, motion_state->velocity, 0.0f);
            }
        }
    }

    if (motion_state->ground_bounces != 0.0f) {
        if (motion_state->ground_bounces < 4.0f) {
            fast_vec_scale(motion_state->velocity, motion_state->velocity, 0.7f);
        }
        if (motion_state->ground_bounces < 7.0f) {
            motion_state->ground_bounces = motion_state->ground_bounces + 1.0f;
            motion_state->gravity = motion_state->gravity * 0.75f;
        }
    }
    if (MOBY(moby)->pos.x < 2.0f || 1021.0f < MOBY(moby)->pos.x || MOBY(moby)->pos.y < 2.0f ||
        1021.0f < MOBY(moby)->pos.y || MOBY(moby)->pos.z < 2.0f || 1021.0f < MOBY(moby)->pos.z) {
        motion_outcome_flags = 0x100;
    }

    if (motion_state->landing_animation_frame != -1.0f &&
        motion_state->apex_animation_frame != -1.0f) {
        switch (motion_state->phase) {
        case MOTION_RISING: {
            float ratio;
            ratio = absolute_float(motion_state->velocity[2] / motion_state->gravity);
            if (2.0f < ratio) {
                motion_or_record_value = compute_interpolated_record_value(moby);
                rr = (motion_state->apex_animation_frame - motion_or_record_value) / ratio;
                if (0.0f < rr) {
                    approach_value((float *)&MOBY(moby)->anim_speed, rr, frame_scale * 0.1f);
                }
            }
            break;
        }
        case MOTION_FALLING:
            motion_or_record_value = compute_interpolated_record_value(moby);
            if (motion_state->landing_animation_frame <= motion_or_record_value) {
                *(int *)(moby + 0x58) = 0;
            } else {
                float negative_half_gravity_step = motion_state->gravity * -0.5f;
                landing_prediction_divisor = 60.0f;
                result_count =
                    solve_quadratic_equation(&larger_quadratic_root,
                                             &smaller_quadratic_root,
                                             negative_half_gravity_step,
                                             motion_state->velocity[2] - negative_half_gravity_step,
                                             MOBY(moby)->pos.z - ground_reference_height);
                if (result_count > 0 && 0.0f < larger_quadratic_root &&
                    larger_quadratic_root < landing_prediction_divisor) {
                    landing_prediction_divisor = larger_quadratic_root;
                }
                if (landing_prediction_divisor < 0.0001f) {
                    landing_prediction_divisor = 0.0001f;
                }
                motion_or_record_value = compute_interpolated_record_value(moby);
                rr = (motion_state->landing_animation_frame - motion_or_record_value) /
                     landing_prediction_divisor;
                if (2.0f < rr) {
                    rr = 2.0f;
                } else if (rr < 0.0f) {
                    rr = 0.0f;
                }
                approach_value((float *)&MOBY(moby)->anim_speed, rr, frame_scale * 0.1f);
            }
            break;
        case MOTION_GROUNDED:
            MOBY(moby)->anim_speed = 1.0f;
            break;
        }
    }

    if (MOBY(moby)->anim_flags & 2) {
        motion_outcome_flags |= 0x40;
    }
    if (!(motion_state->flags & 4)) {
        FUN_L00_00258278(moby,
                         &motion_state->turn_velocity,
                         motion_state->turn_target,
                         0.02f,
                         0.3f,
                         motion_state->max_turn_step);
    }

    i = 0;
    qcopy(eye, &MOBY(moby)->pos);
    eye[2] = eye[2] + motion_state->collision_z_offset;
    fast_vec_add(&MOBY(moby)->pos, &MOBY(moby)->pos, motion_state->velocity);
    qcopy(collision_probe_position, &MOBY(moby)->pos);
    collision_probe_position[2] = collision_probe_position[2] + motion_state->collision_z_offset;
    fast_vec_add(predicted_query_position, collision_probe_position, motion_state->velocity);
    result_count = func_L00_001F2BE8_d6f0(
        convert_integer_to_float(motion_state->collision_radius_1024ths) * 0.0010742187732830644f,
        predicted_query_position,
        16,
        moby,
        0);
    if (i < result_count) {
        int **pp = D_L00_00177F00;
        int   k = result_count;
        do {
            if (is_moby_in_class_range_500_to_540(*pp) == 0) {
                if (motion_state->flags & 0x20) {
                    if (motion_state->phase == 2) {
                        motion_outcome_flags |= 0x100;
                    }
                }
            } else {
                spawn_nearby_effect((char *)*pp,
                                    1.0f,
                                    (int)moby,
                                    0x10000,
                                    &MOBY(moby)->pos,
                                    motion_state->velocity);
            }
            pp++;
        } while (--k != 0);
    }

    sc = 0.0009765625f;
    motion_or_record_value = fast_vec_length(motion_state->velocity);
    if ((float)motion_state->collision_radius_1024ths * sc < motion_or_record_value) {
        collision_line(eye, collision_probe_position, 36, moby, 0);
        qcopy(collision_probe_position, &D_94C8_60);
    }
    if (func_L00_001F10E0_l(collision_probe_position,
                            convert_integer_to_float(motion_state->collision_radius_1024ths) * sc,
                            36,
                            moby) != 0) {
        motion_outcome_flags |= 2;
        collision_result_position = (Vec4 *)&coll_hit_position;
        qcopy(&MOBY(moby)->pos, collision_result_position);
        one = 1.0f;
        MOBY(moby)->pos.z = MOBY(moby)->pos.z - motion_state->collision_z_offset;
        fast_vec_normalize(surface_normal, (char *)collision_result_position + 0x10, one);
        if (0.7070000171661377f <= surface_normal[2]) {
            float motion_normal_dot = fast_vec_dot(motion_state->velocity, surface_normal);
            if (motion_normal_dot < 0.0f) {
                collision_metadata = (char *)collision_result_position - 0x30;
                if (*(int *)(collision_metadata + 0x18) == 0 ||
                    *(int *)(collision_metadata + 0x1C) > 0) {
                    motion_outcome_flags |= 1;
                    motion_state->phase = 2;
                }
                fast_vec_normalize(normal_response_vector, surface_normal, motion_normal_dot);
                fast_vec_sub(
                    motion_state->velocity, motion_state->velocity, normal_response_vector);
                if (motion_state->phase == 2) {
                    f6c = fast_vec_length(motion_state->velocity);
                    approach_value(&f6c, 0.0f, motion_state->gravity);
                    fast_vec_normalize(motion_state->velocity, motion_state->velocity, f6c);
                }
            }
        } else {
            float normal_response_scale = fast_vec_dot(motion_state->velocity, surface_normal);
            if (normal_response_scale < 0.0f) {
                normal_response_scale = normal_response_scale * -(motion_state->restitution + one);
                fast_vec_normalize(normal_response_vector, surface_normal, normal_response_scale);
                fast_vec_add(
                    motion_state->velocity, motion_state->velocity, normal_response_vector);
            }
        }
    }
    if (motion_state->phase == 0) {
        if (motion_state->velocity[2] < 0.0f) {
            motion_state->phase = 1;
        }
    }
    return motion_outcome_flags;
}
