#include "types.h"
#include "rnc/ui/text/font_metrics.h"
extern s32 get_effect_texture() __asm__("FUN_001f44b8");
extern s32 font_print() __asm__("FUN_001f62b0");
void font_print_large(s32 x, s32 y, s32 color, s32 text, s32 max_chars) __asm__("FUN_001f6530");

void font_print_large(s32 x, s32 y, s32 color, s32 text, s32 max_chars) {
    font_print(x, y, color, text, max_chars, get_effect_texture(1), normal_font_metrics);
}

extern void func_001F6530(s32 x, s32 y, s32 color, s32 text, s32 max_chars)
    __attribute__((alias("FUN_001f6530")));
