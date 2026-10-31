#ifndef LOMBYTE_RNC_UI_TEXT_FONT_FONT_PRINT_H
#define LOMBYTE_RNC_UI_TEXT_FONT_FONT_PRINT_H

#include "types.h"

struct Glyph;

void font_print(s32, s32, u64, u8 *, s32, s64, struct Glyph *) __asm__("FUN_001f62b0");

#endif /* LOMBYTE_RNC_UI_TEXT_FONT_FONT_PRINT_H */
