#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/ui/text/font_state.h"
#include "rnc/gameplay/state/game_globals.h"

struct Glyph {
    u8 u;
    u8 v;
    s8 top;
    s8 adv;
};

#include "rnc/ui/text/font_palette.h"
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void emit_textured_rectangle(f32, f32, f32, f32, s32, s32, s32, s32, u64, s32) __asm__("FUN_001f5808");

void process_bgm_display_text_event(u64 color, u8 *s, s32 n, s32 tex, struct Glyph *g, f32 x, f32 y,
                                    f32 scale) __asm__("FUN_001f6638");

void process_bgm_display_text_event(u64 color, u8 *str, s32 byte_limit, s32 tex, struct Glyph *g, f32 x,
                                    f32 y, f32 scale) {
    u8 *text_cursor;
    s32 i;
    f32 size;
    f32 top;
    f32 dx;
    f32 dy;
    struct Glyph *additional_glyph;
    s32 avg;
    s32 mk;

    if (font_window_active == 0) {
        font_palette_colors[0] = color;
    }
    size = scale * 16.0f;
    i = 0;
    if (byte_limit == 0 || *str == 0) {
        return;
    }
    text_cursor = str;
    do {
        if ((u8)(*text_cursor - 8) < 8) {
            if (GlobalStateFlag != 0) {
                color &= 0xFF000000;
                color |= font_palette_colors[*text_cursor - 8] & 0xFFFFFF;
            }
        } else if (g[*text_cursor].adv != 0) {
            top = convert_integer_to_float(g[*text_cursor].top) * scale;
            if ((u8)(*text_cursor + 0x80) < 0x28) {
                additional_glyph = (struct Glyph *)(((*text_cursor + 0x40) << 2) + (s32)g);
                dx = convert_integer_to_float(additional_glyph->adv) * scale;
                dy = convert_integer_to_float(additional_glyph->top) * scale;
                emit_textured_rectangle(x + dx, y + dy, size, size, additional_glyph->u, additional_glyph->v, 16, 16, color, tex);
            }
            if (*text_cursor < 0x20) {
                avg = (s32)((color & 0xFF) + ((color >> 8) & 0xFF) + ((color >> 16) & 0xFF)) / 3;
                mk = (s32)(color & 0xFF000000);
                mk += avg << 16;
                mk += avg << 8;
                avg += mk;
                emit_textured_rectangle(x, y + top, scale * 24.0f, scale * 16.0f, g[*text_cursor].u, g[*text_cursor].v, 24, 16,
                             avg, tex);
            } else if (*text_cursor > 0x20) {
                emit_textured_rectangle(x, y + top, size, size, g[*text_cursor].u, g[*text_cursor].v, 16, 16, color, tex);
            }
            x += convert_integer_to_float(g[*text_cursor].adv) * scale;
        }
        i++;
        if (i == byte_limit) {
            break;
        }
        text_cursor++;
    } while (*text_cursor != 0);
}

extern __typeof__(process_bgm_display_text_event) func_001F6638
    __attribute__((alias("FUN_001f6638")));
