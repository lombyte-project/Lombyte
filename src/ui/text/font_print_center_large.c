#include "types.h"
extern u8 D_001DF790[];
extern s32 get_effect_texture() __asm__("func_001F44B8");
extern s32 measure_text_width_large() __asm__("func_001F6290");
extern void font_print() __asm__("func_001F62B0");
s32 font_print_center_large(s32 center_x, s32 y, s32 color, s32 text,
                            s32 max_chars) __asm__("FUN_001f6c20");

s32 font_print_center_large(s32 center_x, s32 y, s32 color, s32 text, s32 max_chars) {
    s32 left_x;

    left_x = center_x - (measure_text_width_large(text, max_chars) >> 1);
    font_print(left_x, y, color, text, max_chars, get_effect_texture(3), D_001DF790);
    return left_x;
}

extern __typeof__(font_print_center_large) func_001F6C20 __attribute__((alias("FUN_001f6c20")));
