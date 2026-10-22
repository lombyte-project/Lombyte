#include "types.h"
extern f32 func_001F99C0(f32);
f32 cam_interp_values(f32 from, f32 to, f32 stiffness, f32 damping, f32 max,
                      f32 *vel) __asm__("FUN_001ebd78");

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
    if (*retained_step > func_001F99C0(delta)) {
        *retained_step = func_001F99C0(delta);
    } else if (*retained_step < -func_001F99C0(delta)) {
        *retained_step = -func_001F99C0(delta);
    }
    return from + *retained_step;
}

extern __typeof__(cam_interp_values) func_001EBD78 __attribute__((alias("FUN_001ebd78")));
