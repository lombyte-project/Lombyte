#include "types.h"
extern u8 D_001DF790[];
extern s32 get_effect_texture() __asm__("FUN_001f44b8");
extern s32 measure_text_width_large() __asm__("FUN_001f6290");
extern s32 font_print() __asm__("FUN_001f62b0");
void font_print_right_large(s32 right_x, s32 y, s32 color, s32 text,
                            s32 length) __asm__("FUN_001f6a60");

void font_print_right_large(s32 right_x, s32 y, s32 color, s32 text, s32 length) {
    s32 x;

    x = right_x - measure_text_width_large(text, length);
    font_print(x, y, color, text, length, get_effect_texture(3), D_001DF790);
}

extern void func_001F6A60(s32 right_x, s32 y, s32 color, s32 text, s32 length)
    __attribute__((alias("FUN_001f6a60")));
