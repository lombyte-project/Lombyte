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
extern f32 func_001FA6C0(s32);
extern void FUN_001f5808(f32, f32, f32, f32, s32, s32, s32, s32, u64, s32);

void process_bgm_display_text_event(u64 color, u8 *s, s32 n, s32 tex, struct Glyph *g, f32 x, f32 y,
                                    f32 scale) __asm__("FUN_001f6638");

void process_bgm_display_text_event(u64 color, u8 *str, s32 n, s32 tex, struct Glyph *g, f32 x,
                                    f32 y, f32 scale) {
    u8 *s;
    s32 i;
    f32 size;
    f32 top;
    f32 dx;
    f32 dy;
    struct Glyph *e;
    s32 avg;
    s32 mk;

    if (D_0015F4A0 == 0) {
        D_0018CAF8[0] = color;
    }
    size = scale * 16.0f;
    i = 0;
    if (n == 0 || *str == 0) {
        return;
    }
    s = str;
    do {
        if ((u8)(*s - 8) < 8) {
            if (D_0015F49C != 0) {
                color &= 0xFF000000;
                color |= D_0018CAF8[*s - 8] & 0xFFFFFF;
            }
        } else if (g[*s].adv != 0) {
            top = func_001FA6C0(g[*s].top) * scale;
            if ((u8)(*s + 0x80) < 0x28) {
                e = (struct Glyph *)(((*s + 0x40) << 2) + (s32)g);
                dx = func_001FA6C0(e->adv) * scale;
                dy = func_001FA6C0(e->top) * scale;
                FUN_001f5808(x + dx, y + dy, size, size, e->u, e->v, 16, 16, color, tex);
            }
            if (*s < 0x20) {
                avg = (s32)((color & 0xFF) + ((color >> 8) & 0xFF) + ((color >> 16) & 0xFF)) / 3;
                mk = (s32)(color & 0xFF000000);
                mk += avg << 16;
                mk += avg << 8;
                avg += mk;
                FUN_001f5808(x, y + top, scale * 24.0f, scale * 16.0f, g[*s].u, g[*s].v, 24, 16,
                             avg, tex);
            } else if (*s > 0x20) {
                FUN_001f5808(x, y + top, size, size, g[*s].u, g[*s].v, 16, 16, color, tex);
            }
            x += func_001FA6C0(g[*s].adv) * scale;
        }
        i++;
        if (i == n) {
            break;
        }
        s++;
    } while (*s != 0);
}

extern __typeof__(process_bgm_display_text_event) func_001F6638
    __attribute__((alias("FUN_001f6638")));
