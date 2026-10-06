#include "types.h"
extern void draw_rect_overlay(s32, s32, s32, s32, u64) __asm__("func_001F52A0");
void draw_outlined_rect(s32 x, s32 y, s32 w, s32 h, s32 color) __asm__("FUN_001f6060");

void draw_outlined_rect(s32 x, s32 y, s32 w, s32 h, s32 color) {
    s32 fill;
    s32 x_in;
    s32 x_off;
    s32 w_in;
    s32 w_left;
    s32 h_out;
    s32 h_top;
    s32 h_in;
    s32 y_out;
    s32 y_in;
    s32 y_top;
    s32 y_off;
    s32 x_out;
    s32 w_left2;
    s32 h_off;

    fill = (color & 0xFF000000) | 4;
    draw_rect_overlay(x, y, w, h, fill);
    x_in = x + 1;
    x_off = x - 5;
    w_in = w + 3;
    w_left = w - 1;
    h_out = h + 5;
    h_top = h_out;
    h_in = h - 3;
    draw_rect_overlay(x - 1, x_in, w_in, h_out, color);
    y_out = y + 3;
    x_out = x + 3;
    draw_rect_overlay(x - 3, x_off, w_left, h_in, color);
    y_in = y - 3;
    y_top = y_in;
    y_off = y + 1;
    draw_rect_overlay(x_off, y_in, w_left, w + 1, color);
    w_left2 = w - 3;
    h_off = h + 1;
    draw_rect_overlay(x_out, y_off, w_left2, w - 5, color);
    draw_rect_overlay(y - 1, y_off, w_left2, h_in, color);
    draw_rect_overlay(y_out, y + 5, w_in, h_off, color);
    draw_rect_overlay(x_out, y_out, h - 1, h_off, color);
    draw_rect_overlay(x_in, y_top, h + 3, h_top, color);
}

extern __typeof__(draw_outlined_rect) func_001F6060 __attribute__((alias("FUN_001f6060")));
