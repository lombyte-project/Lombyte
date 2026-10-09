#ifndef LOMBYTE_RNC_UI_TEXT_FONT_METRICS_H
#define LOMBYTE_RNC_UI_TEXT_FONT_METRICS_H

#include "types.h"

/* 232 glyph records of 4 bytes (u, v, y offset, advance) per font. */
extern u8 normal_font_metrics[0x3A0] __asm__("D_001DF050");
extern u8 small_font_metrics[0x3A0] __asm__("D_001DF3F0");
extern u8 large_font_metrics[0x3A0] __asm__("D_001DF790");

#endif /* LOMBYTE_RNC_UI_TEXT_FONT_METRICS_H */
