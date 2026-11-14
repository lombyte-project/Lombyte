#include "types.h"
#include "rnc/math/asm_helpers.h"

f32 round_float_to_decimal_places(s32 decimal_places, f32 input_value) __asm__("FUN_00214c48");

f32 round_float_to_decimal_places(s32 decimal_places, f32 input_value) {
    s32 remaining_decimal_places;
    s32 decimal_scale_factor;
    f32 scale;
    f32 half;

    remaining_decimal_places = decimal_places;
    decimal_scale_factor = 1;
    if (remaining_decimal_places > 0) {
        do {
            remaining_decimal_places--;
            decimal_scale_factor *= 10;
        } while (remaining_decimal_places != 0);
    }
    scale = (f32)decimal_scale_factor;
    half = 1.0f / (2.0f * scale);
    input_value = input_value + half;
    input_value = input_value * scale;
    input_value = (f32)truncate_float_to_s32(input_value);
    return input_value / scale;
}

extern __typeof__(round_float_to_decimal_places) func_00214C48
    __attribute__((alias("FUN_00214c48")));
