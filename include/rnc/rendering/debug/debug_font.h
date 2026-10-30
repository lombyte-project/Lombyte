#ifndef LOMBYTE_RNC_RENDERING_DEBUG_DEBUG_FONT_H
#define LOMBYTE_RNC_RENDERING_DEBUG_DEBUG_FONT_H

#include "types.h"

/* Debug font advance width of each character 0x20..0x7F
   (print_debug_text_centered clamps the index below 0x60). */
extern s32 font_char_widths[0x60] __asm__("D_00189DC0");

#endif /* LOMBYTE_RNC_RENDERING_DEBUG_DEBUG_FONT_H */
