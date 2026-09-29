#include "types.h"

typedef struct {
    u8 pad_0[0x40];
    f32 w;
    f32 h;
} Font;

typedef struct {
    u8 pad_0[0x78];
    Font *font;
} FontHolder;

typedef struct {
    u8 pad_0[0x40];
    void *owner;
} MenuFocus;

typedef struct {
    u8 pad_0[0x4];
    MenuFocus *focus;
    u8 pad_8[0x28];
    s32 slot[65];
    s32 unk134;
    s32 unk138;
} MenuState;

typedef struct {
    s32 slot;
    u8 pad_4[0x48];
} ItemInfo;

typedef struct {
    u16 icon;
    s16 frame;
    s16 kind;
    s16 id;
    u16 pad_8;
} Cell;

typedef struct {
    u8 pad_0[0x14];
    FontHolder *holder;
    u8 pad_18[0x8];
    s32 w;
    s32 h;
    u8 pad_28[0x8];
    s32 flags;
    f32 margin_x;
    f32 margin_y;
    s32 sel;
    s32 rows;
    s32 cols;
    Cell *cells;
} Grid;

extern MenuState D_001D5BF0;
extern ItemInfo D_001863D8[];
extern u8 D_0013D388[];
extern u8 D_0013D4C0[];
extern u8 D_0013E520[];
extern s32 D_0015F438;
extern s32 D_001601B0 __attribute__((sda));
extern f32 D_00160290 __attribute__((sda));
extern f32 D_00160294 __attribute__((sda));

extern s32 SubtractIntegerWithClamp(s32);
extern void func_001F4280(s32);
extern void func_001F4398(void);
extern s32 FUN_001ff960(s32, s32);
extern void func_00200080(s32, s32, s32, s32, s32, s32);
extern void func_00200E08(s32, s32, s32, s32, u64, s32);
extern s32 func_00233980(s32, s64) __asm__("FUN_00233980");

s32 fun_0021d948(Grid *g) __asm__("FUN_0021d948");

s32 fun_0021d948(Grid *g) {
    Font *font;
    Cell *c;
    s32 focused;
    f32 x0, xstep, y0, y, ystep, x;
    f32 scale;
    s32 m;
    s32 icw, ich;
    s32 i, j;
    s32 ix, iy, ix2, iy2;
    s32 id, off;
    u32 color;

    font = g->holder->font;
    focused = D_001D5BF0.focus->owner == g;
    c = g->cells;
    func_00233980(0x42, 0x8000000044L);
    func_00233980(0x47, 0xB);
    func_001F4280(0);

    if (g->cols >= 2) {
        x0 = g->margin_x;
        xstep = D_00160290 + (font->w - (x0 + x0) - D_00160290 * g->cols) / (g->cols - 1);
    } else {
        xstep = 0.0f;
        x0 = (font->w - D_00160290) * 0.5f;
    }

    if (g->flags & 2) {
        ystep = D_00160294 + 0.15f;
        y0 = g->margin_y;
    } else if (g->rows >= 2) {
        y0 = g->margin_y;
        ystep = D_00160294 + (font->h - (y0 + y0) - D_00160294 * g->rows) / (g->rows - 1);
    } else {
        ystep = 0.0f;
        y0 = (font->h - D_00160294) * 0.5f;
    }

    m = g->h;
    if (m < g->w) {
        m = g->w;
    }
    scale = (f32)(m << 4) / (font->h < font->w ? font->w : font->h);
    icw = scale * D_00160290;
    ich = scale * D_00160294;

    y = y0;
    for (i = 0; i < g->rows; i++) {
        x = x0;
        for (j = 0; j < g->cols; j++) {
            iy = scale * y;
            iy2 = iy + ich;
            ix = scale * x;
            ix2 = ix + icw;
            if (focused && g->sel == c - g->cells) {
                color = ((SubtractIntegerWithClamp((D_0015F438 & 0x3F) - 0x20) + 0x40) * 0x10202) | 0x80000000;
                func_00200E08(ix - 0x30, iy - 0x30, ix2 + 0x30, iy2 + 0x30, color, 1);
                func_00200E08(ix - 0x10, iy - 0x10, ix2 + 0x10, iy2 + 0x10, D_001601B0, 1);
            }
            if (c->kind == 0 ? D_0013D4C0[c->id] : D_0013D388[c->id]) {
                off = 0;
                if ((u16)c->kind == 0) {
                    id = c->id;
                    if (D_001D5BF0.slot[D_001863D8[id].slot] == id && !(g->flags & 0x20)) {
                        off = 1;
                    }
                    if (off == 0) {
                        off = D_0013E520[id] ? 4 : 0;
                    }
                    if (D_001D5BF0.unk134 != 0 && (g->flags & 8)) {
                        off = 2;
                    }
                    if (D_001D5BF0.unk138 != 0 && (g->flags & 4)) {
                        off = 2;
                    }
                }
                func_00200080(FUN_001ff960(c->icon, c->frame + off), ix, iy, icw, ich, 0x80);
            }
            c++;
            x += xstep;
        }
        y += ystep;
    }
    func_001F4398();
    return 2;
}

extern __typeof__(fun_0021d948) func_0021D948 __attribute__((alias("FUN_0021d948")));
