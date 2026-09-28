#include "types.h"
extern f32 func_001F99C0(f32);
f32 cam_interp_values(f32 from, f32 to, f32 stiffness, f32 damping, f32 max, f32 *vel) __asm__("FUN_001ebd78");

f32 cam_interp_values(f32 from, f32 to, f32 stiffness, f32 damping, f32 max, f32 *vel) {
    f32 delta;

    delta = to - from;
    *vel = *vel + (stiffness * delta - damping * *vel);
    if (max != 0.0f) {
        if (*vel > max) {
            *vel = max;
        } else if (*vel < -max) {
            *vel = -max;
        }
    }
    if (*vel > func_001F99C0(delta)) {
        *vel = func_001F99C0(delta);
    } else if (*vel < -func_001F99C0(delta)) {
        *vel = -func_001F99C0(delta);
    }
    return from + *vel;
}

extern __typeof__(cam_interp_values) func_001EBD78 __attribute__((alias("FUN_001ebd78")));
