#include "types.h"
extern u8 D_001DF050[];
extern s32 get_effect_texture() __asm__("FUN_001f44b8");
extern s32 font_print_window() __asm__("FUN_001f7090");
void font_print_window_regular(s32 box, s32 color, s32 text, s32 max_chars) __asm__("FUN_001f7580");

void font_print_window_regular(s32 box, s32 color, s32 text, s32 max_chars) {
    font_print_window(box, color, text, max_chars, get_effect_texture(1), D_001DF050);
}

extern void func_001F7580(s32 box, s32 color, s32 text, s32 max_chars)
    __attribute__((alias("FUN_001f7580")));
