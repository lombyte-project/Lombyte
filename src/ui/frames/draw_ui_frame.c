#include "types.h"

extern void func_001F52A0(s32, s32, s32, s32, u64);

void draw_ui_frame(s32 x, s32 y, s32 w, s32 h, s32 color) __asm__("FUN_001f5f18");

void draw_ui_frame(s32 x, s32 y, s32 w, s32 h, s32 color) {
    s32 fill;
    s32 x_in;
    s32 x_out;
    s32 y_in;
    s32 y_out;
    s32 w_out;
    s32 w_top;
    s32 h_off;
    s32 h_top;

    fill = (color << 24) | 0x40404;
    x_in = x + 1;
    w_out = w - 2;
    func_001F52A0(x, y, w, h, fill);
    x_out = x + 2;
    y_in = y - 1;
    w_top = w - 3;
    func_001F52A0(x_in, y_in, w_out, w, fill);
    y_out = y - 2;
    func_001F52A0(x_out, y_out, w_top, w_out, fill);
    x += 4;
    y -= 4;
    func_001F52A0(x, y, w - 4, w_top, fill);
    h_off = h + 2;
    h_top = h + 3;
    func_001F52A0(x_in, y_in, h, h_off, fill);
    func_001F52A0(x_out, y_out, h_off, h_top, fill);
    func_001F52A0(x, y, h_top, h + 4, fill);
}

extern __typeof__(draw_ui_frame) func_001F5F18 __attribute__((alias("FUN_001f5f18")));
