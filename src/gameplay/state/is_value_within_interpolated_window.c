#include "types.h"
struct InterpolatedAnim {
    u8 pad_0[0x58];
    f32 unk58;
    f32 unk5C;
};

extern f32 compute_interpolated_record_value() __asm__("func_0020C9E0");
extern f32 round_float_to_decimal_places(s32, f32) __asm__("func_00214C48");
s32 is_value_within_interpolated_window(struct InterpolatedAnim *anim, f32 value) __asm__("FUN_00214cc8");

s32 is_value_within_interpolated_window(struct InterpolatedAnim *anim, f32 value) {
    f32 interp_value;
    f32 window;
    f32 diff;
    s32 result;

    interp_value = compute_interpolated_record_value();
    diff = round_float_to_decimal_places(4, interp_value - value);
    window = round_float_to_decimal_places(4, anim->unk58 * anim->unk5C);
    result = 0;
    if (value <= interp_value) {
        result = 1;
        if (!(diff < window)) {
            result = 0;
        }
    }
    return result;
}
