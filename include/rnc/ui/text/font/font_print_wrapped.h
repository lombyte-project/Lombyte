#ifndef LOMBYTE_RNC_UI_TEXT_FONT_FONT_PRINT_WRAPPED_H
#define LOMBYTE_RNC_UI_TEXT_FONT_FONT_PRINT_WRAPPED_H

#include "types.h"

struct Glyph;

s32 font_print_wrapped(s32, s32, s32, s32, u64, u8 *, s32, u64, struct Glyph *) __asm__("FUN_001f6cb8");

#endif /* LOMBYTE_RNC_UI_TEXT_FONT_FONT_PRINT_WRAPPED_H */
