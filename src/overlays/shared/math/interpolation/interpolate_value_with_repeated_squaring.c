/* Eases a value between two ends as x crosses a range, the weight squared repeatedly (the fighter jet steering). */
#include "types.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B8FC0.c: func_L13_002BB2F8), where it is exact; names translated to the US level program. */

float interpolate_value_with_repeated_squaring(int   flag,
                                               int   squaring_loop_limit,
                                               float x,
                                               float start_value,
                                               float end_value,
                                               float lo,
                                               float hi) __asm__("FUN_L13_002ba088");

float interpolate_value_with_repeated_squaring(int   flag,
                                               int   squaring_loop_limit,
                                               float x,
                                               float start_value,
                                               float end_value,
                                               float lo,

                                               float hi) {
    float end_value_weight;
    float start_value_weight;
    float interpolated_value = 0.0f;
    int   i;
    if (x > lo) {
        end_value_weight = (x - lo) / (hi - lo);
        if (end_value_weight < 0.0f) {
            end_value_weight = 0.0f;
        }
        if (end_value_weight > 1.0f) {
            end_value_weight = 1.0f;
        }
        for (i = 1; i < squaring_loop_limit; i++) {
            end_value_weight = end_value_weight * end_value_weight;
        }
        start_value_weight = 1.0f - end_value_weight;
        if (flag != 0) {
            interpolated_value = fast_add_rotations(
                start_value, fast_subtract_rotations(end_value, start_value) * end_value_weight);
        } else {
            interpolated_value = start_value * start_value_weight + end_value * end_value_weight;
        }
    } else {
        interpolated_value = start_value;
    }
    return interpolated_value;
}
