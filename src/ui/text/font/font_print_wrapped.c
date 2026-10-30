#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"

struct Glyph {
    u8 u;
    u8 v;
    s8 yoff;
    s8 adv;
};

#include "rnc/ui/text/font_palette.h"

extern void draw_textured_quad(s32 x,
                               s32 y,
                               s32 w,
                               s32 h,
                               s32 u,
                               s32 v,
                               s32 uw,
                               s32 vh,
                               u64 color,
                               u64 extra) __asm__("FUN_001f5450");

s32 font_print_wrapped(s32           x,
                       s32           y,
                       s32           width,
                       s32           height,
                       u64           color,
                       u8           *text,
                       s32           len,
                       u64           extra,
                       struct Glyph *font) __asm__("FUN_001f6cb8");

s32 font_print_wrapped(s32 x,
                       s32 y,
                       s32 width,
                       s32 height,
                       u64 color,
                       u8 *text,
                       s32 text_byte_limit,
                       u64 extra,

                       struct Glyph *font) {
    s32           i;
    s32           j;
    s32           cx;
    s32           cy;
    u8           *current_text_byte;
    u32           character_code;
    f32           start;
    f32           lookahead_width;
    struct Glyph *supplemental_glyph;

    cx = x;
    cy = y;
    i = 0;
    font_palette_colors[0] = color;
    while (i != text_byte_limit && text[i] != 0 && y + height >= cy + 16) {
        j = i;
        lookahead_width = 0.0f;
        current_text_byte = &text[i];
        start = cx;
        while (j != text_byte_limit && text[j] != ' ' && text[j] >= 0x10) {
            lookahead_width += font[text[j]].adv;
            j++;
        }
        if ((f32)(x + width) < start + lookahead_width) {
            if (*current_text_byte != ' ' && *current_text_byte >= 0x10) {
                i--;
            }
            cx = x;
            cy += 16;
        } else if ((u8)(*current_text_byte - 8) < 8) {
            if (font_color_codes_enabled != 0) {
                color &= 0xFF000000;
                color |= font_palette_colors[*current_text_byte - 8] & 0xFFFFFF;
            }
        } else {
            character_code = *current_text_byte;
            if (font[character_code].adv != 0) {
                if ((u8)(character_code + 0x80) < 0x28) {
                    supplemental_glyph =
                        (struct Glyph *)(((character_code + 0x40) << 2) + (s32)font);
                    draw_textured_quad(cx + supplemental_glyph->adv,
                                       cy + supplemental_glyph->yoff,
                                       16,
                                       16,
                                       supplemental_glyph->u,
                                       supplemental_glyph->v,
                                       16,
                                       16,
                                       color,
                                       extra);
                }
                if (*current_text_byte < 0x20) {
                    draw_textured_quad(cx,
                                       cy + font[*current_text_byte].yoff,
                                       0x18,
                                       16,
                                       font[*current_text_byte].u,
                                       font[*current_text_byte].v,
                                       0x18,
                                       16,
                                       color,
                                       extra);
                } else if (*current_text_byte > 0x20) {
                    draw_textured_quad(cx,
                                       cy + font[*current_text_byte].yoff,
                                       16,
                                       16,
                                       font[*current_text_byte].u,
                                       font[*current_text_byte].v,
                                       16,
                                       16,
                                       color,
                                       extra);
                }
                cx += font[*current_text_byte].adv;
            }
        }
        i++;
    }
    return cy - y + 16;
}

extern __typeof__(font_print_wrapped) func_001F6CB8 __attribute__((alias("FUN_001f6cb8")));
