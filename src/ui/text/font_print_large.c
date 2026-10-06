#include "types.h"
extern u8 D_001DF050[];
extern s32 get_effect_texture() __asm__("FUN_001f44b8");
extern s32 font_print() __asm__("FUN_001f62b0");
void font_print_large(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) __asm__("FUN_001f6530");

void font_print_large(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    font_print(arg0, arg1, arg2, arg3, arg4, get_effect_texture(1), D_001DF050);
}

extern void func_001F6530(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
    __attribute__((alias("FUN_001f6530")));
