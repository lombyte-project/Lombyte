#include "types.h"
extern s32 font_print_center_large(s32, s32, u64, s32, s32) __asm__("func_001F6C20");
extern void draw_stretchable_ui_frame(s32, s32, s32, s32, s32) __asm__("func_00201128");
void draw_framed_text(s32 x, s32 y, s32 color, s32 text) __asm__("FUN_00201200");

void draw_framed_text(s32 x, s32 y, s32 color, s32 text) {
    s32 alpha = (s32)color >> 24;
    s32 text_left;
    s32 left;

    if (alpha > 0x50) {
        alpha = 0x50;
    }
    text_left = font_print_center_large(x + 1, y + 1, (s64)color & (s64)(s32)0xFF000000, text, -1);
    left = text_left - 0x20;
    draw_stretchable_ui_frame(left, y - 8, (x - left) * 2, 0x20, alpha);
    font_print_center_large(x, y, color, text, -1);
}
extern __typeof__(draw_framed_text) func_00201200 __attribute__((alias("FUN_00201200")));
