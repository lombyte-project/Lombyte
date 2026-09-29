#include "types.h"

/* Draws a vertical text menu: picks the font size from the flags, sizes
   the rows, then prints each item (and its optional subtitle) with the
   focused row highlighted, scrolling the box to keep it visible.
   Returns 1 once after flag 0x8000 is consumed, else 2. */

typedef struct {
    short s[12];
} TextBox;

typedef struct {
    s16 text;
    s16 enabled;
    s32 id;
    s16 subtext;
    s16 padA;
} MenuItem;

typedef struct {
    u8 pad0[0x20];
    s32 width;
    s32 height;
    u8 pad28[8];
    s32 flags;
    MenuItem *items;
    u8 pad38[8];
    s32 sel;
    s32 scroll;
} Menu;

typedef struct {
    u8 pad0[0x40];
    Menu *focus;
} MenuState;

extern MenuState *D_001D5BF4[];
extern u8 D_0013D408[];
extern s32 D_0015ED88;
extern u8 D_001DF050[];
extern u8 D_001DF3F0[];
extern u8 D_001DF790[];

extern void FUN_00233980(s32, long);
extern void func_001F4280(s32);
extern void func_001F4398(void);
extern s32 FUN_001f44b8(s32);
extern void func_001F61E8(void);
extern void func_001F61F8(void);
extern void FUN_001f7090(TextBox *, long, char *, s32, s32, u8 *);
extern char *func_001FDD10(s32);
extern void func_0021F8E8(s32, s32, s32);
extern void *memset(void *, int, unsigned int);

s32 FUN_0021d4a8(Menu *menu) {
    s32 size;
    s32 kind;
    s32 focused;
    u8 *font;
    s32 n;
    MenuItem *p;
    s32 rowh;
    s32 glyphs;
    s32 i;
    s32 sel;
    s32 en;
    s32 color;
    char *text;
    s32 end;
    s32 y;

    size = 12;
    kind = 1;
    font = D_001DF050;
    focused = D_001D5BF4[0]->focus == menu;
    if (menu->flags & 4) {
        size = 14;
        kind = 3;
        font = D_001DF790;
    }
    if (menu->flags & 8) {
        size = 10;
        kind = 2;
        font = D_001DF3F0;
    }
    FUN_00233980(0x42, 0x44);
    FUN_00233980(0x47, 0x2004B);
    func_001F4280(0);

    n = 0;
    p = menu->items;
    while (p->text != 0) {
        p++;
        n++;
    }
    if (menu->flags & 0x10) {
        rowh = size + 3;
    } else {
        rowh = menu->height / (n + 1);
    }
    y = rowh - size / 2;
    {
        TextBox box = { { 4, menu->height - 4, 0, menu->width - 2, 0,
                          y - menu->scroll, 0, 0, size + 2 } };

        glyphs = FUN_001f44b8(kind);
        for (i = 0; menu->items[i].text != 0; i++) {
            sel = 0;
            if (focused && menu->sel == i) {
                sel = 1;
            }
            en = menu->items[i].enabled != 0;
            if (menu->flags & 2) {
                color = 0x80FFA888;
            } else if (sel) {
                color = en ? 0x8020FFFF : 0x80006060;
            } else {
                color = en ? 0x80FFA888 : 0x80303030;
            }
            if (!(menu->flags & 0x10000) && sel && box.s[5] < 4) {
                menu->scroll -= 4;
            }
            text = func_001FDD10(menu->items[i].text);
            box.s[4] = (menu->flags & 0xA00) ? 0x20 : 4;
            if (menu->flags & 0x400) {
                box.s[9] = 1;
                box.s[4] = menu->width >> 1;
            }
            if (sel) {
                func_001F61F8();
            }
            FUN_001f7090(&box, color, text, -1, glyphs, font);
            if (sel) {
                func_001F61E8();
            }
            if (menu->flags & 0x200) {
                func_0021F8E8(0xF, box.s[5] + 9, D_0013D408[i] != 0);
            }
            if (menu->flags & 0x800) {
                func_0021F8E8(0xF, box.s[5] + 9, D_0015ED88 == menu->items[i].id);
            }
            box.s[5] += box.s[7];
            if (menu->items[i].subtext != 0) {
                text = func_001FDD10(menu->items[i].subtext);
                box.s[4] = 0x14;
                FUN_001f7090(&box, color, text, -1, glyphs, font);
                box.s[5] += rowh;
            }
            box.s[5] += 8;
            if (!(menu->flags & 0x10000) && sel) {
                end = box.s[5] + box.s[7];
                if (box.s[1] < end) {
                    if (menu->flags & 0x8000) {
                        menu->scroll += end - box.s[1];
                    } else {
                        menu->scroll += 4;
                    }
                }
            }
        }
    }
    func_001F4398();
    if (menu->flags & 0x8000) {
        menu->flags ^= 0x8000;
        return 1;
    }
    return 2;
}

extern __typeof__(FUN_0021d4a8) func_0021D4A8 __attribute__((alias("FUN_0021d4a8")));
