#include "types.h"

struct Glyph {
    u8 u;
    u8 v;
    s8 yoff;
    s8 adv;
};

extern s32 D_0018CAF8[];
extern s32 D_0015F49C;
extern void draw_textured_quad(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, u64 color,
                               u64 extra) __asm__("FUN_001f5450");

s32 font_print_wrapped(s32 x, s32 y, s32 width, s32 height, u64 color, u8 *text, s32 len, u64 extra,
                       struct Glyph *font) __asm__("FUN_001f6cb8");

s32 font_print_wrapped(s32 x, s32 y, s32 width, s32 height, u64 color, u8 *text, s32 len, u64 extra,
                       struct Glyph *font) {
    s32 i;
    s32 j;
    s32 cx;
    s32 cy;
    u8 *p;
    u32 c;
    f32 start;
    f32 w;
    struct Glyph *g;

    cx = x;
    cy = y;
    i = 0;
    D_0018CAF8[0] = color;
    while (i != len && text[i] != 0 && y + height >= cy + 16) {
        j = i;
        w = 0.0f;
        p = &text[i];
        start = cx;
        while (j != len && text[j] != ' ' && text[j] >= 0x10) {
            w += font[text[j]].adv;
            j++;
        }
        if ((f32)(x + width) < start + w) {
            if (*p != ' ' && *p >= 0x10) {
                i--;
            }
            cx = x;
            cy += 16;
        } else if ((u8)(*p - 8) < 8) {
            if (D_0015F49C != 0) {
                color &= 0xFF000000;
                color |= D_0018CAF8[*p - 8] & 0xFFFFFF;
            }
        } else {
            c = *p;
            if (font[c].adv != 0) {
                if ((u8)(c + 0x80) < 0x28) {
                    g = (struct Glyph *)(((c + 0x40) << 2) + (s32)font);
                    draw_textured_quad(cx + g->adv, cy + g->yoff, 16, 16, g->u, g->v, 16, 16, color,
                                       extra);
                }
                if (*p < 0x20) {
                    draw_textured_quad(cx, cy + font[*p].yoff, 0x18, 16, font[*p].u, font[*p].v,
                                       0x18, 16, color, extra);
                } else if (*p > 0x20) {
                    draw_textured_quad(cx, cy + font[*p].yoff, 16, 16, font[*p].u, font[*p].v, 16,
                                       16, color, extra);
                }
                cx += font[*p].adv;
            }
        }
        i++;
    }
    return cy - y + 16;
}

extern __typeof__(font_print_wrapped) func_001F6CB8 __attribute__((alias("FUN_001f6cb8")));
