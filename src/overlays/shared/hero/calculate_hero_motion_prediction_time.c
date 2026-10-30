/* Hero: the time ahead used to predict the hero's motion. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "asm.h"

#include "eetypes.h"
#include "qcopy.h"
#include "rnc/overlay/level_labels.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/world/collision/collision_line.h"

typedef int V4q_32b90 __attribute__((mode(TI), aligned(16)));

typedef union {
    V4q_32b90 q;
    f32       f[4];
} V4;

/* D_0013F350, the shared Level-00 state block. Offsets are taken off the
   listing: 0x80 is a 16-byte value (lq/sq) whose .f[1] is also read as a
   scalar, 0xE0 likewise with .f[2], 0x270 is a 16-byte value, and 0x20B3 is
   the u8 the switch is on. */
extern u8   D_L06_00174600_lui[] __asm__("D_L06_00174600") NOT_SDA;
extern void clip_negative_segment_endpoint_xyz(V4 *, void *) __asm__("FUN_L00_0025d238");
extern f32  compute_gravity_relative_vector_inclination(void *) __asm__("FUN_L00_00233d30");

f32 calculate_hero_motion_prediction_time(f32 prediction_time,
                                          f32 acceleration_scale,
                                          f32 target_z) __asm__("FUN_L06_00242950");

f32 calculate_hero_motion_prediction_time(f32 prediction_time,
                                          f32 acceleration_scale,

                                          f32 target_z) {
    enum {
        HERO_MOTION_Z_AXIS_ACCELERATION = 0,
        HERO_MOTION_STORED_VECTOR_ACCELERATION = 1,
        HERO_MOTION_FIXED_POINT_ACCELERATION = 2
    };
    f32 predicted_position[4];
    V4  position_or_acceleration_vector;
    V4  vector_scratch;
    V4  hero_position_copy;
    f32 quadratic_roots[6];
    f32 x, y, z;

    switch (hero.gravity_mode) {
    case HERO_MOTION_Z_AXIS_ACCELERATION:
        if (target_z < 0.0f) {
            fast_vec_scale(predicted_position, &hero.motion.velocity, prediction_time);
            predicted_position[2] -=
                ((prediction_time * prediction_time + prediction_time) * 0.5f) * acceleration_scale;
            fast_vec_add(predicted_position, predicted_position, &hero.motion.pos);
            position_or_acceleration_vector.q = hero.motion.pos.q;
            clip_negative_segment_endpoint_xyz(&position_or_acceleration_vector,
                                               predicted_position);
            qcopy(&position_or_acceleration_vector, &hero.motion.pos);
            if (collision_line(&position_or_acceleration_vector, predicted_position, 0xd24, 0, 0) !=
                0) {
                if (compute_gravity_relative_vector_inclination((f32 *)D_L06_00174600_lui) <=
                    0.87266463f) {
                    target_z = *(f32 *)(D_L06_00174600_lui - 0x18);
                }
            }
        }
        if (target_z > 0.0f) {
            if (solve_quadratic_equation(&quadratic_roots[0],
                                         &quadratic_roots[1],
                                         acceleration_scale * -0.5f,
                                         hero.motion.velocity.f[2] - acceleration_scale * -0.5f,
                                         hero.motion.pos.f[2] - target_z) > 0 &&
                quadratic_roots[0] > 0.0f) {
                return quadratic_roots[0];
            }
        }
        break;
    case HERO_MOTION_STORED_VECTOR_ACCELERATION:
        qcopy(&position_or_acceleration_vector, &hero.ground_normal);
        fast_vec_normalize(
            &position_or_acceleration_vector, &position_or_acceleration_vector, acceleration_scale);
        fast_vec_scale(&position_or_acceleration_vector, &position_or_acceleration_vector, -1.0f);
        fast_vec_scale(predicted_position, &hero.motion.velocity, prediction_time);
        fast_vec_scale(&vector_scratch,
                       &position_or_acceleration_vector,
                       ((prediction_time * prediction_time + prediction_time) * 0.5f) *
                           acceleration_scale);
        fast_vec_add(predicted_position, &vector_scratch, predicted_position);
        fast_vec_add(predicted_position, &hero.motion.pos, predicted_position);
        hero_position_copy.q = hero.motion.pos.q;
        clip_negative_segment_endpoint_xyz(&hero_position_copy, predicted_position);
        if (collision_line(&hero.motion.pos, predicted_position, 0xd24, 0, 0) != 0) {
            f32 nb;
            nb = acceleration_scale * -0.5f;
            z = gravity_z(&hero.motion.velocity) - nb;
            x = gravity_z(&hero.motion.pos);
            y = gravity_z(&D_L06_001745E0);
            if (solve_quadratic_equation(&quadratic_roots[2], &quadratic_roots[3], nb, z, x - y) >
                    0 &&
                quadratic_roots[2] > 0.0f) {
                return quadratic_roots[2];
            }
        }
        break;
    case HERO_MOTION_FIXED_POINT_ACCELERATION:
        vector_scratch.f[2] = 100.0f;
        vector_scratch.f[1] = 100.0f;
        vector_scratch.f[0] = 100.0f;
        fast_vec_sub(&position_or_acceleration_vector, &hero.motion.pos, &vector_scratch);
        fast_vec_normalize(
            &position_or_acceleration_vector, &position_or_acceleration_vector, acceleration_scale);
        fast_vec_scale(&position_or_acceleration_vector, &position_or_acceleration_vector, -1.0f);
        fast_vec_scale(predicted_position, &hero.motion.velocity, prediction_time);
        fast_vec_scale(&vector_scratch,
                       &position_or_acceleration_vector,
                       ((prediction_time * prediction_time + prediction_time) * 0.5f) *
                           acceleration_scale);
        fast_vec_add(predicted_position, &vector_scratch, predicted_position);
        fast_vec_add(predicted_position, &hero.motion.pos, predicted_position);
        hero_position_copy.q = hero.motion.pos.q;
        clip_negative_segment_endpoint_xyz(&hero_position_copy, predicted_position);
        if (collision_line(&hero.motion.pos, predicted_position, 0xd24, 0, 0) != 0) {
            f32 nb;
            nb = acceleration_scale * -0.5f;
            z = gravity_z(&hero.motion.velocity) - nb;
            x = gravity_z(&hero.motion.pos);
            y = gravity_z(&D_L06_001745E0);
            if (solve_quadratic_equation(&quadratic_roots[4], &quadratic_roots[5], nb, z, x - y) >
                    0 &&
                quadratic_roots[4] > 0.0f) {
                return quadratic_roots[4];
            }
        }
        break;
    }
    return prediction_time;
}
