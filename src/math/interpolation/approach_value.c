#include "types.h"
#include "rnc/math/fast_math.h"

f32 approach_value(f32 target, f32 step, f32 *value) __asm__("FUN_00213ed8");

f32 approach_value(f32 target, f32 step, f32 *current_value) {
    f32 value_delta = target - *current_value;

    if (value_delta > step) {
        value_delta = step;
    } else if (value_delta < -step) {
        value_delta = -step;
    }
    *current_value += value_delta;
    return absolute_float(target - *current_value);
}

extern __typeof__(approach_value) func_00213ED8 __attribute__((alias("FUN_00213ed8")));
