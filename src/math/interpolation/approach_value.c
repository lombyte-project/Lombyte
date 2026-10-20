#include "types.h"
extern f32 func_001F99C0(f32);
f32 approach_value(f32 target, f32 step, f32 *value) __asm__("FUN_00213ed8");

f32 approach_value(f32 target, f32 step, f32 *value) {
    f32 value_delta = target - *value;

    if (value_delta > step) {
        value_delta = step;
    } else if (value_delta < -step) {
        value_delta = -step;
    }
    *value += value_delta;
    return func_001F99C0(target - *value);
}

extern __typeof__(approach_value) func_00213ED8 __attribute__((alias("FUN_00213ed8")));
