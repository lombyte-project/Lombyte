#include "types.h"
extern u8 D_001DF3F0[];
extern s32 get_effect_texture() __asm__("func_001F44B8");
extern s32 measure_text_width_small() __asm__("func_001F6270");
extern void font_print() __asm__("func_001F62B0");
s32 font_print_center_small(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                            s32 arg4) __asm__("FUN_001f6b88");

s32 font_print_center_small(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 temp_16_21;

    temp_16_21 = arg0 - (measure_text_width_small(arg3, arg4) >> 1);
    font_print(temp_16_21, arg1, arg2, arg3, arg4, get_effect_texture(2), D_001DF3F0);
    return temp_16_21;
}

extern __typeof__(font_print_center_small) func_001F6B88 __attribute__((alias("FUN_001f6b88")));
