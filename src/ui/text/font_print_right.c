#include "types.h"
extern u8 D_001DF050[];
extern s32 get_effect_texture() __asm__("FUN_001f44b8");
extern s32 measure_text_width_regular() __asm__("FUN_001f6250");
extern s32 font_print() __asm__("FUN_001f62b0");
void font_print_right(s32 right_x, s32 y, s32 color, s32 text, s32 length) __asm__("FUN_001f6940");

void font_print_right(s32 right_x, s32 y, s32 color, s32 text, s32 length) {
    s32 x;

    x = right_x - measure_text_width_regular(text, length);
    font_print(x, y, color, text, length, get_effect_texture(1), D_001DF050);
}

extern void func_001F6940(s32 right_x, s32 y, s32 color, s32 text, s32 length)
    __attribute__((alias("FUN_001f6940")));
