#include "types.h"

extern f32 absolute_float(f32) __asm__("func_001F99C0");
extern f32 square_root_scalar(f32) __asm__("func_001F9988");
extern f32 approach_value(f32 target, f32 step, f32 *value) __asm__("FUN_00213ed8");
f32 advance_accelerated_scalar(f32 *value, f32 *velocity, f32 target, f32 acceleration_step,
                               f32 braking_step, f32 maximum_speed) __asm__("FUN_00213f38");

/* Advance value toward target while retaining velocity between calls. Brake
   when motion points away from the target; otherwise choose a desired speed
   from velocity squared divided by twice the braking step. Near the stopping
   distance, one path brakes at 1.1 times the usual step. Snap to target when
   the next step reaches it. Return the displacement applied this call. */
f32 advance_accelerated_scalar(f32 *value, f32 *velocity, f32 target, f32 acceleration_step,
                               f32 braking_step, f32 maximum_speed) {
    f32 distance;
    f32 stopping_distance;
    f32 desired_speed;
    f32 remaining_distance;

    distance = target - *value;
    if (*velocity * distance >= 0.0f && distance != 0.0f) {
        stopping_distance = *velocity * *velocity / braking_step * 0.5f;
        if (absolute_float(distance) < stopping_distance) {
            if (stopping_distance < absolute_float(distance) + absolute_float(*velocity)) {
                approach_value(0.0f, braking_step, velocity);
            } else {
                approach_value(0.0f, braking_step * 1.1f, velocity);
            }
        } else {
            desired_speed = square_root_scalar(2.0f * braking_step * distance);
            if (maximum_speed < desired_speed) {
                desired_speed = maximum_speed;
            }
            if (distance < 0.0f) {
                approach_value(-desired_speed, acceleration_step, velocity);
            } else {
                approach_value(desired_speed, acceleration_step, velocity);
            }
        }
        remaining_distance = absolute_float(distance);
        if (absolute_float(*velocity) < remaining_distance) {
            *value += *velocity;
            return *velocity;
        }
        *value = target;
        return distance;
    }
    approach_value(0.0f, braking_step, velocity);
    *value += *velocity;
    return *velocity;
}

extern __typeof__(advance_accelerated_scalar) func_00213F38 __attribute__((alias("FUN_00213f38")));

