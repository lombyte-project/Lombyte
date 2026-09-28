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
extern void func_001F5450(s32, s32, s32, s32, s32, s32, s32, s32, s64, s64);

void font_print(s32 x, s32 y, u64 color, u8 *str, s32 n, s64 tex, struct Glyph *g) __asm__("FUN_001f62b0");

void font_print(s32 x, s32 y, u64 color, u8 *str, s32 n, s64 tex, struct Glyph *g) {
    s32 i;
    u8 *s;
    u8 c;
    struct Glyph *e;
    s32 avg;
    s32 mk;

    if (D_0015F4A0 == 0) {
        D_0018CAF8[0] = color;
    }
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
        } else {
            c = *s;
            if (g[c].adv != 0) {
                if ((u8)(c + 0x80) < 0x28) {
                    e = (struct Glyph *)(((c + 0x40) << 2) + (s32)g);
                    func_001F5450(x + e->adv, y + e->top, 16, 16, e->u, e->v, 16, 16, color, tex);
                }
                if (*s < 0x20) {
                    avg = (s32)((color & 0xFF) + ((color >> 8) & 0xFF) + ((color >> 16) & 0xFF)) / 3;
                    mk = (s32)(color & 0xFF000000);
                    mk += avg << 16;
                    mk += avg << 8;
                    avg += mk;
                    func_001F5450(x, y + g[*s].top, 24, 16, g[*s].u, g[*s].v, 24, 16, avg, tex);
                } else if (*s > 0x20) {
                    func_001F5450(x, y + g[*s].top, 16, 16, g[*s].u, g[*s].v, 16, 16, color, tex);
                }
                x += g[*s].adv;
            }
        }
        i++;
        if (i == n) {
            break;
        }
        s++;
    } while (*s != 0);
}

extern __typeof__(font_print) func_001F62B0 __attribute__((alias("FUN_001f62b0")));
