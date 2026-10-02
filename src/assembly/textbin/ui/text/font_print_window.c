#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/text/font_print_window/FUN_001f7090.s", FUN_001f7090);
#else
#include "types.h"

struct Glyph {
    u8 u;
    u8 v;
    s8 top;
    s8 adv;
};

struct FontWin {
    s16 top;
    s16 bottom;
    s16 left;
    s16 right;
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    s16 line_h;
    u16 flags;
    s16 ofs_x;
    s16 ofs_y;
};

struct ScreenOfs { s32 w; s32 h; };

extern s32 D_0015F4A0;
extern s32 D_0015F49C[];
extern s32 D_0018CAF8[];
extern struct ScreenOfs D_0013E500;
extern void func_00233A40(s32, s32, s32, s32);
extern s32 func_001F6200(u8 *, s32, struct Glyph *);
extern void func_001F62B0(s32, s32, u64, u8 *, s32, s64, struct Glyph *);
extern void func_001F6638(s32, u8 *, s32, s64, struct Glyph *, f32, f32, f32);

void font_print_window(struct FontWin *w, u64 color, u8 *str, s32 n, s64 tex, struct Glyph *glyphs) __asm__("FUN_001f7090");

void font_print_window(struct FontWin *w, u64 color, u8 *str, s32 n, s64 tex, struct Glyph *glyphs) {
    s16 start[32];
    s16 end[32];
    s16 colors[32];
    s32 total;
    s32 total0;
    s32 retried;
    s32 maxlines;
    s32 lastw;
    s32 count;
    s32 next;
    s32 curcolor;
    s32 pos;
    s32 wrap;
    s32 wsum;
    s32 adv;
    s32 e;
    s32 a;
    s32 b;
    s32 three;
    s32 k;
    s32 y;
    s32 len;
    s32 width;
    u8 *p;
    f32 fx;
    struct FontWin *win;

    func_00233A40(w->left, w->right - 1, w->top, w->bottom - 1);
    D_0015F4A0 = 1;
    if ((w->flags ^ 1) & 1) {
        total = w->right - w->x;
    } else {
        a = w->right - w->x;
        b = w->x - w->left;
        if (a < b) {
            b = a;
        }
        total = b * 2;
    }
    total0 = total;
    curcolor = 0;
    retried = 0;
    maxlines = 0;
    lastw = 0;
    three = 3;
retry:
    count = 0;
    pos = 0;
    while (pos != n && str[pos] != 0) {
            next = count + 1;
            start[count] = pos;
            colors[count] = curcolor;
            wrap = pos;
            wsum = 0;
            if (total > 0) {
                do {
                    if (str[pos] == ' ' || str[pos] < 0x10) {
                        wrap = pos;
                    }
                    if (D_0015F49C[0] != 0 && (str[pos] >= 8 && str[pos] < 0x10)) {
                        curcolor = str[pos] - 8;
                    }
                    if (str[pos] < 2) {
                        break;
                    }
                    adv = glyphs[str[pos++]].adv;
                    if (adv != 0) {
                        wsum += adv;
                    }
                } while (wsum < total);
            }
            end[count] = wrap;
            if (end[count] == start[count]) {
                end[count] = pos;
            }
            pos = end[count];
            if (str[pos] == ' ' || str[pos] < 0x10) {
                end[count]--;
            }
            count = next;
            if (str[pos] == 0) {
                break;
                lastw = wsum;
            }
            pos++;
    }
    if (!retried && count >= 2) {
        if (maxlines == 0) {
            maxlines = count;
        }
        if (maxlines < count) {
            total = total0;
            retried = 1;
            goto retry;
        }
        if (lastw < total / three) {
            total -= 0x10;
            goto retry;
        }
    }

    win = w;
    len = win->line_h * count;
    win->w = 0;
    y = win->y;
    win->h = len;
    if (win->flags & 2) {
        y -= len >> 1;
    }
    for (k = 0; k < count; k++, y += win->line_h) {
        if (y + win->line_h < win->top) {
            continue;
        }
        if (win->bottom < y) {
            continue;
        }
        len = end[k] - start[k] + 1;
        width = func_001F6200(str + start[k], len, glyphs);
        if (win->w < width) {
            win->w = width;
        }
        if (win->flags & 4) {
            continue;
        }
        D_0018CAF8[0] = color;
        if (win->flags & 8) {
            if (win->flags & 1) {
                fx = (f32)(win->x - (width >> 1)) + (f32)win->ofs_x * 0.0625f;
            } else {
                fx = (f32)win->x + (f32)win->ofs_x * 0.0625f;
            }
            func_001F6638(D_0018CAF8[colors[k]], str + start[k], len, tex, glyphs,
                          fx, (f32)y + (f32)win->ofs_y * 0.0625f, 1.0f);
        } else if (win->flags & 1) {
            func_001F62B0(win->x - (width >> 1), y, D_0018CAF8[colors[k]], str + start[k], len, tex, glyphs);
        } else {
            func_001F62B0(win->x, y, D_0018CAF8[colors[k]], str + start[k], len, tex, glyphs);
        }
    }
    D_0015F4A0 = 0;
    func_00233A40(0, D_0013E500.w - 1, 0, D_0013E500.h - 1);
}
#endif /* NON_MATCHING */
