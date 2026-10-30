#include "types.h"
#include "rnc/math/fast_math.h"

f32 cam_interp_values(f32 from, f32 to, f32 stiffness, f32 damping, f32 max, f32 *vel) __asm__("FUN_001ebd78");

f32 cam_interp_values(f32 from, f32 to, f32 stiffness, f32 damping, f32 max, f32 *retained_step) {
    f32 delta;

    delta = to - from;
    *retained_step = *retained_step + (stiffness * delta - damping * *retained_step);
    if (max != 0.0f) {
        if (*retained_step > max) {
            *retained_step = max;
        } else if (*retained_step < -max) {
            *retained_step = -max;
        }
    }
    if (*retained_step > absolute_float(delta)) {
        *retained_step = absolute_float(delta);
    } else if (*retained_step < -absolute_float(delta)) {
        *retained_step = -absolute_float(delta);
    }
    return from + *retained_step;
}

extern __typeof__(cam_interp_values) func_001EBD78 __attribute__((alias("FUN_001ebd78")));
