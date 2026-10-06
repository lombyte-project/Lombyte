#include "types.h"
extern s32 D_00189DC0[];
extern void print_debug_text(s32, s32, s32, u8 *) __asm__("FUN_001f0bd0");

s32 print_debug_text_centered(s32 x, s32 y, s32 color, u8 *text) __asm__("FUN_001f0c50");

s32 print_debug_text_centered(s32 x, s32 y, s32 color, u8 *text) {
    s32 width;
    u8 *p;
    u8 c;
    s32 idx;

    width = 0;
    p = text;
    while (*p != 0) {
        c = *p++ - 0x20;
        idx = c;
        if (c >= 0x60) {
            idx = 0x20;
        }
        width += D_00189DC0[idx];
    }
    x -= width >> 1;
    print_debug_text(x, y, color, text);
    return x;
}

extern __typeof__(print_debug_text_centered) func_001F0C50 __attribute__((alias("FUN_001f0c50")));
