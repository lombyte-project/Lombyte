#ifndef LOMBYTE_RNC_UI_TEXT_FONT_PALETTE_H
#define LOMBYTE_RNC_UI_TEXT_FONT_PALETTE_H

#include "types.h"

/* Text colors picked by the control bytes 8..15 of a string. The text
   printers store the caller's color in entry 0 (when text color changes are
   not locked) and OR each selected entry's RGB into the running color. */
extern s32 font_palette_colors[8] __asm__("D_0018CAF8");

#endif /* LOMBYTE_RNC_UI_TEXT_FONT_PALETTE_H */
