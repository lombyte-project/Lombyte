#include "types.h"
#include "rnc/ui/text/font_metrics.h"
extern s32 get_effect_texture() __asm__("func_001F44B8");
extern s32 measure_text_width_small() __asm__("func_001F6270");
extern void font_print() __asm__("func_001F62B0");
s32 font_print_center_small(s32 center_x, s32 y, s32 color, s32 text,
                            s32 max_chars) __asm__("FUN_001f6b88");

s32 font_print_center_small(s32 center_x, s32 y, s32 color, s32 text, s32 max_chars) {
    s32 left_x;

    left_x = center_x - (measure_text_width_small(text, max_chars) >> 1);
    font_print(left_x, y, color, text, max_chars, get_effect_texture(2), small_font_metrics);
    return left_x;
}

extern __typeof__(font_print_center_small) func_001F6B88 __attribute__((alias("FUN_001f6b88")));
