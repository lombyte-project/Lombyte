#include "types.h"
#include "rnc/math/asm_helpers.h"

f32 round_float_to_decimal_places(s32 decimal_places, f32 input_value) __asm__("FUN_00214c48");

f32 round_float_to_decimal_places(s32 decimal_places, f32 input_value) {
    s32 count;
    s32 value;
    f32 scale;
    f32 half;

    count = decimal_places;
    value = 1;
    if (count > 0) {
        do {
            count--;
            value *= 10;
        } while (count != 0);
    }
    scale = (f32)value;
    half = 1.0f / (2.0f * scale);
    input_value = input_value + half;
    input_value = input_value * scale;
    input_value = (f32)truncate_float_to_s32(input_value);
    return input_value / scale;
}

extern __typeof__(round_float_to_decimal_places) func_00214C48
    __attribute__((alias("FUN_00214c48")));
