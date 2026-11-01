#ifndef LOMBYTE_RNC_UI_TEXT_FONT_H
#define LOMBYTE_RNC_UI_TEXT_FONT_H

#include "types.h"

struct Glyph;

void font_print(s32, s32, u64, u8 *, s32, s64, struct Glyph *) __asm__("FUN_001f62b0");
s32  font_print_center(s32, s32, s32, s32, s32) __asm__("FUN_001f6af0");
s32  font_print_center_large(s32, s32, s32, s32, s32) __asm__("FUN_001f6c20");
s32  font_print_wrapped(s32, s32, s32, s32, u64, u8 *, s32, u64, struct Glyph *) __asm__("FUN_001f6cb8");
void font_set_window(void *, int, int, int, int, int, int, int, int);
void load_debug_font(void) __asm__("FUN_001e9338");

#endif /* LOMBYTE_RNC_UI_TEXT_FONT_H */
