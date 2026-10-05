#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00213f38/FUN_00213f38.s", FUN_00213f38);
#else
#include "types.h"
extern f32 AbsoluteFloat(f32) __asm__("func_001F99C0");
extern f32 square_root_scalar(f32) __asm__("func_001F9988");
extern void approach_value(f32 *, f32, f32) __asm__("func_00213ED8");
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
    f32 distance_or_next_value;
    f32 desired_velocity;
    f32 velocity_step;
    f32 desired_speed;
    distance = target - (*value);
    if ((((*velocity) * distance) >= 0.0f) && (distance != 0.0f)) {
        stopping_distance = (((*velocity) * (*velocity)) / braking_step) * 0.5f;
        if (AbsoluteFloat(distance) < stopping_distance) {
            distance_or_next_value = AbsoluteFloat(distance);
            distance_or_next_value = distance_or_next_value + AbsoluteFloat(*velocity);
            if (stopping_distance < distance_or_next_value) {
                desired_velocity = 0.0f;
                velocity_step = braking_step;
                approach_value(velocity, desired_velocity, velocity_step);
            } else {
                approach_value(velocity, 0.0f, braking_step * 1.1f);
                goto tail;
            }
        } else {
            desired_speed = square_root_scalar((2.0f * braking_step) * distance);
            if (maximum_speed < desired_speed) {
                desired_speed = maximum_speed;
            }
            if (distance < 0.0f) {
                desired_velocity = -desired_speed;
                velocity_step = acceleration_step;
                approach_value(velocity, desired_velocity, velocity_step);
            } else {
                approach_value(velocity, desired_speed, acceleration_step);
            }
        }
    tail:
        distance_or_next_value = AbsoluteFloat(distance);

        if (AbsoluteFloat(*velocity) < distance_or_next_value) {
            *value = (*value) + (*velocity);
            return *velocity;
        }
        *value = target;
        return distance;
    }
    approach_value(velocity, 0.0f, braking_step);
  *value = (distance_or_next_value = *value + *velocity, distance_or_next_value);
    return *velocity;
}

extern __typeof__(advance_accelerated_scalar) func_00213F38 __attribute__((alias("FUN_00213f38")));

#endif /* NON_MATCHING */
