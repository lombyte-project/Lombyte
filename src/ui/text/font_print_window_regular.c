#include "types.h"
extern u8 D_001DF050[];
extern s32 get_effect_texture() __asm__("FUN_001f44b8");
extern s32 font_print_window() __asm__("FUN_001f7090");
void font_print_window_regular(s32 arg0, s32 arg1, s32 arg2, s32 arg3) __asm__("FUN_001f7580");

void font_print_window_regular(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    font_print_window(arg0, arg1, arg2, arg3, get_effect_texture(1), D_001DF050);
}

extern void func_001F7580(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
    __attribute__((alias("FUN_001f7580")));
