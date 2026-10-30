/* Level 18 floating_platform: tilt, bob, sink and sparks. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/game/usage_stats.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/level_spline.h"
#include "rnc/audio/voice_alloc.h"

void update_floating_platform_tilt_and_bob(struct Moby *moby) __asm__("FUN_L18_002d8710");
void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

#include "qcopy.h"

extern int scale_game_frames(int);

extern float D_L18_0015F638 __attribute__((sda));

#include "rnc/overlay/l18/mobys/floating_platform_data/floating_platform_update_2.h"

#include "rnc/overlay/l18/mobys/floating_platform_globals.h"

extern float floating_platform_sink_trigger_distance_gp __asm__("D_L18_00161A5C")
    __attribute__((sda));

#include "rnc/overlay/l18/mobys/floating_platform_data/floating_platform_update.h"

extern int D_L18_001623A8 __attribute__((sda));
void       spawn_floating_platform_sparks(struct Moby *moby) __asm__("FUN_L18_002d88b8");

extern void  spawn_floating_platform_sparks(struct Moby *) __asm__("FUN_L18_002d88b8");
extern void  hero_set_state(int, int) __asm__("FUN_L18_00227dd0");
extern void  update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern int   random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern float random_angle_radians(void) __asm__("FUN_00213308");

/* Ported from rac1-decomp nonmatching/l18_veldin2/func_L18_002D96B0.c (func_L18_002D96B0) */
/* Sinking platform update: arms its tilt, then sinks it under the hero until it is removed. */
void floating_platform_update(struct Moby *moby) __asm__("FUN_L18_002d82c0");

void floating_platform_update(struct Moby *moby) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_WAIT_FOR_TILT_REQUEST = 1,
        STATE_WAIT_FOR_SINK_REQUEST = 2,
        STATE_TILT_DELAY = 3,
        STATE_WAIT_FOR_SINK_TRIGGER = 4,
        STATE_SINKING = 5
    };
    char *data;
    float delta[4] __attribute__((aligned(16)));
    float old_rot[4] __attribute__((aligned(16)));
    float tilt_rate_scale;
    float rate;
    float sink;
    float angle;

    data = (char *)moby->pvars;
    fast_vec_scale(delta, &moby->pos, -1.0f);
    qcopy(old_rot, &moby->rot);
    switch (moby->state) {
    case STATE_INITIALIZE:
        *(float *)(data + 0x60) = moby->pos.z;
        *(float *)(data + 0x64) = moby->pos.z;
        *(int *)(data + 0x70) = 0;
        *(int *)(data + 0x68) = 0;
        *(int *)(data + 0x6C) = 0;
        angle = random_angle_radians();
        *(int *)(data + 0x5C) |= 1;
        *(float *)(data + 0x74) = angle;
        if (*(int *)(data + 0x88) != 0) {
            moby->state = 2;
        } else {
            moby->state = 1;
        }
        break;
    case STATE_TILT_DELAY:
        if (fast_dec_timer((int *)(data + 0x7C)) == 0) {
            break;
        }
        tilt_rate_scale = (convert_integer_to_float(D_L18_001623A8) -
                           floating_platform_motion_scale_input_start_gp) /
                          (floating_platform_motion_scale_input_end_gp -
                           floating_platform_motion_scale_input_start_gp);
        if (1.0f < tilt_rate_scale) {
            tilt_rate_scale = 1.0f;
        } else if (tilt_rate_scale < 0.0f) {
            tilt_rate_scale = 0.0f;
        }
        tilt_rate_scale = 1.0f - tilt_rate_scale * 0.333f;
        if (*(int *)(data + 0x84) != 0) {
            rate = floating_platform_tilt_rate_degrees_config_84_nonzero_gp * tilt_rate_scale;
        } else {
            rate = floating_platform_tilt_rate_degrees_config_84_zero_gp * tilt_rate_scale;
        }
        *(float *)(data + 0x6C) = rate * DEG_TO_RAD * frame_time;
        moby->state = 4;
        break;
    case STATE_WAIT_FOR_SINK_TRIGGER:
        if (moby->visible != 0 && fast_dec_timer((int *)(data + 0x8C)) != 0) {
            spawn_floating_platform_sparks(moby);
            *(int *)(data + 0x8C) = scale_game_frames(random_int_between(0x5A, 0x78));
        }
        if (hero.ground_moby == moby && hero.air_frames.s == 0) {
            *(int *)(data + 0x8C) = 0;
            moby->state = 5;
            *(short *)(data + 0x92) = allocate_voice_for_target_entry(1, 0, moby);
        }
        if (*(int *)(data + 0x80) == 0) {
            break;
        }
        if (vector_distance_xy(&moby->pos, &hero.motion.pos) <
            floating_platform_sink_trigger_distance_gp) {
            moby->state = 5;
            *(short *)(data + 0x92) = allocate_voice_for_target_entry(1, 0, moby);
        }
        break;
    case STATE_SINKING:
        if (moby->visible != 0 && fast_dec_timer((int *)(data + 0x8C)) != 0) {
            spawn_floating_platform_sparks(moby);
            *(int *)(data + 0x8C) = scale_game_frames(random_int_between(0xF, 0x1E));
        }
        sink = (convert_integer_to_float(D_L18_001623A8) -
                floating_platform_motion_scale_input_start_gp) /
               (floating_platform_motion_scale_input_end_gp -
                floating_platform_motion_scale_input_start_gp);
        if (1.0f < sink) {
            sink = 1.0f;
        } else if (sink < 0.0f) {
            sink = 0.0f;
        }
        sink = 1.0f - sink * 0.333f;
        if (*(int *)(data + 0x80) != 0 ||
            (*(int *)(data + 0x88) != 0 && *(int *)(data + 0x84) == 0)) {
            *(float *)(data + 0x70) -= frame_time_sq * 15.0f;
        } else if (*(int *)(data + 0x84) != 0) {
            *(float *)(data + 0x70) -=
                floating_platform_sink_acceleration_config_84_nonzero_gp * sink * frame_time_sq;
        } else {
            *(float *)(data + 0x70) -=
                floating_platform_sink_acceleration_config_84_zero_gp * sink * frame_time_sq;
        }
        *(float *)(data + 0x64) += *(float *)(data + 0x70);
        if (moby->pos.z < *(float *)(data + 0x60) - 10.0f) {
            if (hero.ground_moby == moby && hero.air_frames.s == 0) {
                if (hero.motion.pos.f[2] < D_L18_0015F638 && hero.state.current != 0x77) {
                    hero_set_state(0x77, 1);
                }
            }
            *(float *)(data + 0x70) -= frame_time_sq * 13.7f;
            if (moby->pos.z < 15.0f) {
                mark_moby_for_removal((struct Obj *)moby);
                return;
            }
        }
        break;
    case STATE_WAIT_FOR_TILT_REQUEST:
        break;
    case STATE_WAIT_FOR_SINK_REQUEST:
        break;
    default:
        break;
    }
    update_floating_platform_tilt_and_bob(moby);
    fast_vec_add(delta, delta, &moby->pos);
    update_entity_matrix(data + 0x20, delta, old_rot, &moby->rot);
}

extern float floating_platform_bob_amplitude_gp __asm__("D_L18_00161A48") __attribute__((sda));
extern float floating_platform_bob_phase_rate_gp __asm__("D_L18_00161A4C") __attribute__((sda));

void update_floating_platform_tilt_and_bob(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float unclamped_rotation_y = fast_add_rotations(moby->rot.y, *(float *)(data + 0x6C));
    moby->rot.y = unclamped_rotation_y;
    if (moby->state == 5) {
        if (unclamped_rotation_y > 0.5235988f) {
            moby->rot.y = 0.5235988f;
        } else if (unclamped_rotation_y < -0.5235988f) {
            moby->rot.y = -0.5235988f;
        }
    } else if (moby->state == 4) {
        if (unclamped_rotation_y > 0.08726646f) {
            moby->rot.y = 0.08726646f;
        } else if (unclamped_rotation_y < -0.08726646f) {
            moby->rot.y = -0.08726646f;
        }
    } else {
        if (unclamped_rotation_y > 0.034906585f) {
            moby->rot.y = 0.034906585f;
        } else if (unclamped_rotation_y < -0.034906585f) {
            moby->rot.y = -0.034906585f;
        }
    }
    *(float *)(data + 0x74) = fast_add_rotations(
        *(float *)(data + 0x74), floating_platform_bob_phase_rate_gp * DEG_TO_RAD * frame_time);
    moby->pos.z = *(float *)(data + 0x64) +
                  floating_platform_bob_amplitude_gp * fast_sin(*(float *)(data + 0x74));
}

/* Ported from rac1-decomp (src/overlays/l18/vendor_002A8400.c: func_L18_002D9C48), where it is exact; names translated to the US level program. */

typedef struct {
    char          pad0[0x20];
    unsigned char state;
} Level18State;

int request_floating_platform_sink(Level18State *platform_moby) __asm__("FUN_L18_002d8858");

int request_floating_platform_sink(Level18State *platform_moby) {
    if (platform_moby->state == 1) {
        return 1;
    }
    if (platform_moby->state == 2) {
        platform_moby->state = 5;
    }
    return 0;
}

/* Starts the moby's next phase with the given pair if it is waiting. */
int request_floating_platform_tilt(struct Moby *platform_moby,
                                   int          tilt_delay_frames,
                                   int          scaled_frame_count) __asm__("FUN_L18_002d8888");

int request_floating_platform_tilt(struct Moby *platform_moby,
                                   int          tilt_delay_frames,

                                   int scaled_frame_count) {
    int *data = (int *)platform_moby->pvars;
    if (platform_moby->state == 1) {
        platform_moby->state = 3;
        data[0x7C / 4] = tilt_delay_frames;
        data[0x78 / 4] = scaled_frame_count;
        return 1;
    }
    return 0;
}

extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern void  FUN_L00_00257d78(float *, float, float);
extern float FUN_L04_001f1978(float);
extern void  transform_vector_by_basis(void *, void *, void *);

/* Spawns a random ring of particles around the moby. */
extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern int   FUN_001fa6e0(int, int, float);
extern int   random_integer_below(int) __asm__("FUN_00213260");
extern short floating_platform_spark_first_vector_length_base_rate_gp __asm__("D_L18_00161A68")
    __attribute__((sda));
extern short D_L18_00161A6C_gp __asm__("D_L18_00161A6C") __attribute__((sda));
extern short floating_platform_spark_second_vector_downward_z_rate_gp __asm__("D_L18_00161A70")
    __attribute__((sda));
extern short
    floating_platform_spark_vector_length_relative_variation_limit_gp __asm__("D_L18_00161A74")
        __attribute__((sda));
extern short floating_platform_spark_vector_offset_range_end_rate_gp __asm__("D_L18_00161A78")
    __attribute__((sda));
extern short floating_platform_spark_frame_count_random_span_gp __asm__("D_L18_00161A7C")
    __attribute__((sda));
extern short floating_platform_spark_second_frame_count_base_gp __asm__("D_L18_00161A80")
    __attribute__((sda));
extern short floating_platform_spark_third_frame_count_base_gp __asm__("D_L18_00161A84")
    __attribute__((sda));
extern short
    floating_platform_spark_frame_count_relative_variation_limit_gp __asm__("D_L18_00161A88")
        __attribute__((sda));
extern short floating_platform_spark_first_vector_w_range_start_gp __asm__("D_L18_00161A8C")
    __attribute__((sda));
extern short floating_platform_spark_first_vector_w_range_end_gp __asm__("D_L18_00161A90")
    __attribute__((sda));
extern short floating_platform_spark_second_vector_w_range_start_gp __asm__("D_L18_00161A94")
    __attribute__((sda));
extern short floating_platform_spark_second_vector_w_range_end_gp __asm__("D_L18_00161A98")
    __attribute__((sda));
extern short floating_platform_spark_initial_color_range_start_gp __asm__("D_L18_00161A9C")
    __attribute__((sda));
extern short floating_platform_spark_initial_color_range_end_gp __asm__("D_L18_00161AA0")
    __attribute__((sda));
extern short floating_platform_spark_second_color_range_start_gp __asm__("D_L18_00161AA4")
    __attribute__((sda));
extern short floating_platform_spark_second_color_range_end_gp __asm__("D_L18_00161AA8")
    __attribute__((sda));
extern short floating_platform_spark_lookup_index_and_flags_gp __asm__("D_L18_00161AB4")
    __attribute__((sda));

void spawn_floating_platform_sparks(struct Moby *moby) {
    float spark_spawn_position[4];
    float world_emission_direction[4];
    float first_spark_vector[4];
    float second_spark_vector[4];
    float spark_vector_offset[4];
    int   spark_count;
    float spawn_radius;
    float spawn_angle;
    int   i0, i1, i2, i3, i4;
    char *moby_rotation_basis = &moby->rot_row0;
    int   i;

    spark_count = random_integer_below(5) + 5;
    spawn_radius = random_float_between(1.0f, 2.5f);
    spawn_angle = random_angle_radians();
    spark_spawn_position[0] = fast_cos(spawn_angle) * spawn_radius;
    spark_spawn_position[1] = fast_sin(spawn_angle) * spawn_radius;
    spark_spawn_position[2] = 0.0f;
    transform_vector_by_basis(spark_spawn_position, spark_spawn_position, moby_rotation_basis);
    fast_vec_add(spark_spawn_position, spark_spawn_position, &moby->pos);
    set_vector_unit_z(world_emission_direction);
    transform_vector_by_basis(
        world_emission_direction, world_emission_direction, moby_rotation_basis);
    for (i = 0; i < spark_count; i++) {
        FUN_L00_00257d78(spark_vector_offset,
                         0.0f,
                         (*(float *)&(floating_platform_spark_vector_offset_range_end_rate_gp)) *
                             frame_time);
        fast_vec_normalize(
            first_spark_vector,
            world_emission_direction,
            (*(float *)&(floating_platform_spark_first_vector_length_base_rate_gp)) *
                (random_float_between(
                     -(*(float *)&(
                         floating_platform_spark_vector_length_relative_variation_limit_gp)),
                     (*(float *)&(
                         floating_platform_spark_vector_length_relative_variation_limit_gp))) +
                 1.0f) *
                frame_time);
        fast_vec_add(first_spark_vector, first_spark_vector, spark_vector_offset);
        fast_vec_normalize(
            second_spark_vector,
            first_spark_vector,
            (*(float *)&(D_L18_00161A6C_gp)) *
                (random_float_between(
                     -(*(float *)&(
                         floating_platform_spark_vector_length_relative_variation_limit_gp)),
                     (*(float *)&(
                         floating_platform_spark_vector_length_relative_variation_limit_gp))) +
                 1.0f) *
                frame_time);
        second_spark_vector[2] =
            second_spark_vector[2] -
            (*(float *)&(floating_platform_spark_second_vector_downward_z_rate_gp)) * frame_time;
        first_spark_vector[3] = random_float_between(
            (*(float *)&(floating_platform_spark_first_vector_w_range_start_gp)),
            (*(float *)&(floating_platform_spark_first_vector_w_range_end_gp)));
        second_spark_vector[3] = random_float_between(
            (*(float *)&(floating_platform_spark_second_vector_w_range_start_gp)),
            (*(float *)&(floating_platform_spark_second_vector_w_range_end_gp)));
        i0 = FUN_001fa6e0((*(int *)&(floating_platform_spark_initial_color_range_start_gp)),
                          (*(int *)&(floating_platform_spark_initial_color_range_end_gp)),
                          random_float_between(0.0f, 1.0f));
        i1 = FUN_001fa6e0((*(int *)&(floating_platform_spark_second_color_range_start_gp)),
                          (*(int *)&(floating_platform_spark_second_color_range_end_gp)),
                          random_float_between(0.0f, 1.0f));
        i2 = truncate_float_to_s32(FUN_L04_001f1978(
            (float)(*(int *)&(floating_platform_spark_frame_count_random_span_gp)) *
                random_float_between(0.0f, 1.0f) +
            1.0f));
        i3 = truncate_float_to_s32(FUN_L04_001f1978(
            (float)(*(int *)&(floating_platform_spark_second_frame_count_base_gp)) *
            (random_float_between(
                 -(*(float *)&(floating_platform_spark_frame_count_relative_variation_limit_gp)),
                 (*(float *)&(floating_platform_spark_frame_count_relative_variation_limit_gp))) +
             1.0f)));
        i4 = truncate_float_to_s32(FUN_L04_001f1978(
            (float)(*(int *)&(floating_platform_spark_third_frame_count_base_gp)) *
            (random_float_between(
                 -(*(float *)&(floating_platform_spark_frame_count_relative_variation_limit_gp)),
                 (*(float *)&(floating_platform_spark_frame_count_relative_variation_limit_gp))) +
             1.0f)));
        spawn_spark(spark_spawn_position,
                    first_spark_vector,
                    second_spark_vector,
                    i0,
                    i1,
                    i2,
                    i3,
                    i4,
                    (*(int *)&(floating_platform_spark_lookup_index_and_flags_gp)));
    }
}
