#include "types.h"

struct Glyph {
    u8 u;
    u8 v;
    s8 top;
    s8 adv;
};

extern s32 D_0015F4A0;
extern s32 D_0015F49C;
extern s32 D_0018CAF8[];
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64,
                               s64) __asm__("func_001F5450");

void font_print(s32 x, s32 y, u64 color, u8 *text, s32 character_limit, s64 texture,
                struct Glyph *glyphs) __asm__("FUN_001f62b0");

void font_print(s32 x, s32 y, u64 color, u8 *text, s32 character_limit, s64 texture,
                struct Glyph *glyphs) {
    s32 character_count;
    u8 *cursor;
    u8 character;
    struct Glyph *overlay_glyph;
    s32 gray_value;
    s32 gray_color;

    if (D_0015F4A0 == 0) {
        D_0018CAF8[0] = color;
    }
    character_count = 0;
    if (character_limit == 0 || *text == 0) {
        return;
    }
    cursor = text;
    do {
        /* Bytes 8..15 select a palette color, preserving the current alpha. */
        if ((u8)(*cursor - 8) < 8) {
            if (D_0015F49C != 0) {
                color &= 0xFF000000;
                color |= D_0018CAF8[*cursor - 8] & 0xFFFFFF;
            }
        } else {
            character = *cursor;
            if (glyphs[character].adv != 0) {
                /* Characters 0x80..0xa7 also draw the glyph 0x40 entries later. */
                if ((u8)(character + 0x80) < 0x28) {
                    overlay_glyph = (struct Glyph *)(((character + 0x40) << 2) + (s32)glyphs);
                    draw_textured_quad(x + overlay_glyph->adv, y + overlay_glyph->top, 16, 16,
                                       overlay_glyph->u, overlay_glyph->v, 16, 16, color, texture);
                }
                /* Control glyphs use 24-pixel grayscale sprites. */
                if (*cursor < 0x20) {
                    gray_value =
                        (s32)((color & 0xFF) + ((color >> 8) & 0xFF) + ((color >> 16) & 0xFF)) / 3;
                    gray_color = (s32)(color & 0xFF000000);
                    gray_color += gray_value << 16;
                    gray_color += gray_value << 8;
                    gray_value += gray_color;
                    draw_textured_quad(x, y + glyphs[*cursor].top, 24, 16, glyphs[*cursor].u,
                                       glyphs[*cursor].v, 24, 16, gray_value, texture);
                } else if (*cursor > 0x20) {
                    draw_textured_quad(x, y + glyphs[*cursor].top, 16, 16, glyphs[*cursor].u,
                                       glyphs[*cursor].v, 16, 16, color, texture);
                }
                x += glyphs[*cursor].adv;
            }
        }
        character_count++;
        if (character_count == character_limit) {
            break;
        }
        cursor++;
    } while (*cursor != 0);
}

extern __typeof__(font_print) func_001F62B0 __attribute__((alias("FUN_001f62b0")));
