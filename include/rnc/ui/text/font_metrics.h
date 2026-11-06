#ifndef LOMBYTE_RNC_UI_TEXT_FONT_METRICS_H
#define LOMBYTE_RNC_UI_TEXT_FONT_METRICS_H

#include "types.h"

/* One character of a font: where its glyph sits in the font texture, how
 * far it drops below the line and how far the pen moves after it (a glyph
 * with advance 0 is not drawn). */
struct Glyph {
    u8 u;
    u8 v;
    s8 top;
    s8 adv;
};

/* Glyph records by character code, one table per font texture (effect
 * textures 1, 2 and 3). */
extern struct Glyph normal_font_metrics[232] __asm__("D_001DF050");
extern struct Glyph small_font_metrics[232] __asm__("D_001DF3F0");
extern struct Glyph large_font_metrics[232] __asm__("D_001DF790");

#endif /* LOMBYTE_RNC_UI_TEXT_FONT_METRICS_H */
