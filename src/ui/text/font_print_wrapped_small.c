#include "types.h"
extern u8 D_001DF3F0[];
extern s32 get_effect_texture() __asm__("FUN_001f44b8");
extern s32 font_print_wrapped() __asm__("FUN_001f6cb8");
void font_print_wrapped_small(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                              s32 arg6) __asm__("FUN_001f6fd0");

void font_print_wrapped_small(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                              s32 arg6) {
    font_print_wrapped(arg0, arg1, arg2, arg3, arg4, arg5, arg6, get_effect_texture(2), D_001DF3F0);
}

extern void func_001F6FD0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6)
    __attribute__((alias("FUN_001f6fd0")));
