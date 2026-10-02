#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021a328/FUN_0021a328.s", FUN_0021a328);
#else
#include "types.h"
#include "sda.h"

typedef struct {
    short s[12];
} TextBox;

typedef struct {
    u8 pad0[0x20];
    int w;          /* 0x20 */
    int h;          /* 0x24 */
    u8 pad28[8];
    int flags;      /* 0x30 */
    int textId;     /* 0x34 */
    unsigned int stride; /* 0x38 */
    int scroll;     /* 0x3C */
    u8 pad40[4];
    int timer;      /* 0x44 */
    int cur;        /* 0x48 */
    int sub;        /* 0x4C */
} Widget;

typedef struct {
    u8 pad0[4];
    short a;        /* 0x4 */
    short b;        /* 0x6 */
    u8 pad8[2];
} PageEntry;

typedef struct {
    short id;
    u8 pad2[10];
} PageItem;

typedef struct {
    u8 pad0[0x34];
    PageItem *items;   /* 0x34 */
    u8 pad38[4];
    int sel;           /* 0x3C */
    int sel2;          /* 0x40 */
    u8 pad44[4];
    PageEntry *entries; /* 0x48 */
} Page;

typedef struct {
    u8 pad0[0x40];
    Page *page;
} Game;

extern int D_0013CAE0[];
extern u8 D_0013D388[];
extern u8 D_0013D408[];
extern u8 D_0013D4C0[];
extern u8 D_0013E520[];
extern int D_0015ED80 __attribute__((sda));
extern int D_0015ED84 __attribute__((sda));
extern int D_001601B0 __attribute__((sda));
extern int D_001601B4 __attribute__((sda));
extern int D_001601B8 __attribute__((sda));
extern int D_001601BC __attribute__((sda));
extern int D_00160258 __attribute__((sda));
extern int D_00160268 __attribute__((sda));
extern char D_00160270[];
extern char D_00160278[];
extern char D_00160280[];
extern char D_00160288[];
extern int D_001A0314[];
extern Game *D_001D5BF4[];
extern u8 D_001DF050[];
extern u8 D_001DF3F0[];
extern u8 D_001DF790[];

extern void func_001F4280(int);
extern void func_001F4398(void);
extern long func_001F44B8(int);
extern void EnableGlobalStateFlag(void) __asm__("func_001F61E8");
extern void DisableGlobalStateFlag(void) __asm__("func_001F61F8");
extern void func_001F7090(TextBox *, long, char *, int, long, u8 *);
extern int func_001F96F8(int);
extern long func_001FA6E0(int, int, float);
extern char *func_001FDD10(int);
extern int func_001FECC8(short, int, u16 *);
extern long func_0021B6D8(int, long, int);
extern void FUN_00233980(int, long);
extern void *memset(void *, int, unsigned int);
extern int sprintf(char *, const char *, ...);

int FUN_0021a328(Widget *w)
{
    char buf[64];
    u8 *font;
    int fontId;
    char *text;
    int sub;
    int idx;
    int flags;
    int mode;
    int x;
    int y;
    int style;
    long col;
    long color;
    int t;
    Page *page;
    PageEntry *e;
    u8 *tbl;
    int id;

    font = D_001DF050;
    fontId = 1;
    text = D_00160270;
    sub = 0;
    flags = w->flags;
    if (flags & 8) {
        fontId = 3;
        font = D_001DF790;
    }
    if (flags & 0x10) {
        fontId = 2;
        font = D_001DF3F0;
    }
    FUN_00233980(0x42, 0x44);
    FUN_00233980(0x47, 0x2004B);
    flags = w->flags;
    if (flags & 0x20) {
        idx = D_0015ED84 - 1;
        if ((unsigned int)idx >= 0x12) {
            idx = -1;
        }
    } else if (flags & 0x40) {
        idx = D_001A0314[0] - 1;
    } else if (flags & 4) {
        if (w->timer < func_001F96F8(D_001601B4)) {
            w->timer = func_001F96F8(D_001601B4);
        }
        idx = 0;
        w->cur = 0;
        w->sub = 0;
    } else if (flags & 0x80) {
        idx = D_001D5BF4[0]->page->sel2;
        if (flags & 0x8000) {
            sub = D_0015ED80 != 0;
        }
    } else if (flags & 0x100) {
        page = D_001D5BF4[0]->page;
        e = &page->entries[page->sel];
        if (e->a == 0) {
            tbl = D_0013D4C0;
        } else {
            tbl = D_0013D388;
        }
        if (tbl[e->b] == 0) {
            idx = -1;
        } else {
            idx = page->sel;
        }
    } else if (flags & 0x1000) {
        page = D_001D5BF4[0]->page;
        idx = page->sel2;
        {
            short sid = page->items[idx].id;
            w->textId = 0xFFFF;
            func_001FECC8(sid, 1, (u16 *)&w->textId);
        }
    } else {
        page = D_001D5BF4[0]->page;
        id = page->entries[page->sel].b;
        sub = D_0013E520[id] != 0;
        idx = id;
    }

    if (w->timer == -1) {
        w->timer = func_001F96F8(D_001601B4);
        w->cur = idx;
        w->sub = sub;
    }
    if (idx != w->cur) {
        if (func_001F96F8(D_001601B4) < w->timer) {
            w->timer = func_001F96F8(D_001601B4);
        }
        t = w->timer;
        t = t < 1 ? 0 : t - 1;
        t = t < 1 ? 0 : t - 1;
        t = t < 1 ? 0 : t - 1;
        w->timer = t;
        if (t != 0) {
            idx = w->cur;
            sub = w->sub;
        } else {
            w->cur = idx;
            w->sub = sub;
            w->flags &= ~0x400;
            w->scroll = 0;
        }
    } else {
        w->timer += 3;
    }

    flags = w->flags;
    if (flags & 4) {
        if (w->textId == 0) {
            return 1;
        }
        text = func_001FDD10(w->textId);
    } else if (flags & 0x1000) {
        if (w->textId == 0xFFFF) {
            return 1;
        }
        text = func_001FDD10(w->textId);
    } else if ((flags & 0x100) && idx == -1) {
        text = D_00160278;
    } else if (w->textId != 0) {
        text = func_001FDD10(((int *)w->textId + sub)[idx * w->stride / sizeof(int)]);
    }
    if (!(w->flags & 0x11E4) && D_0013D4C0[idx] == 0) {
        text = D_00160278;
    }
    if (w->flags & 0x200) {
        id = *((int *)w->textId + idx * w->stride / sizeof(int));
        if (id != 0x4ED2 && id != 0x4ED9 && id != 0x4EDD) {
            sprintf(buf, D_00160280, func_001FDD10(0x4ECC), text);
            text = buf;
        }
    }

    flags = w->flags;
    mode = flags;
    x = 4;
    y = 4;
    if ((mode & 0x4004) == 0x4004 && w->textId == 0x523E) {
        mode |= 1;
        y = 12;
    }
    if ((flags & 0x800) && D_0013D408[idx] == 0) {
        mode |= 3;
        text = func_001FDD10(0x4F54);
    }
    if (text == 0) {
        text = D_00160288;
    }
    style = 8;
    if (mode & 1) {
        style = 9;
        x = w->w / 2;
    }
    if (mode & 2) {
        style |= 2;
        y = w->h / 2;
    }
    func_001F4280(0);
    col = func_001F44B8(fontId);
    {
        TextBox c = { { D_00160258, w->h - D_00160258, 1, w->w - 4, x,
                        y - (w->scroll >> 4), [8] = D_00160268, style,
                        [11] = -(w->scroll & 0xF) } };

        if (w->flags & 0x10000) {
            c.s[1] = w->h - 1;
        }
        color = func_0021B6D8(w->timer, func_001FA6E0(D_001601B0, 0x80FFA888, 0.5f), 0x80FFA888);
        c.s[9] |= 4;
        func_001F7090(&c, color, text, -1, col, font);
        c.s[9] ^= 4;
        flags = w->flags;
        if (!(flags & 0x2000) && c.s[7] + 4 >= c.s[1] - c.s[0]) {
            if (!(flags & 0x400)) {
                w->flags = flags | 0x400;
                w->scroll = -(w->h * 8);
            }
        } else if (w->flags & 0x400) {
            w->scroll = 0;
            w->flags ^= 0x400;
        }
        c.s[5] = y - (w->scroll >> 4);
        c.s[0] += D_001601BC;
        c.s[1] += D_001601BC;
        c.s[2] += D_001601B8;
        c.s[3] += D_001601B8;
        c.s[4] += D_001601B8;
        c.s[5] += D_001601BC;
        DisableGlobalStateFlag();
        func_001F7090(&c, 0x80000000L, text, -1, col, font);
        EnableGlobalStateFlag();
        c.s[0] -= D_001601BC;
        c.s[1] -= D_001601BC;
        c.s[2] -= D_001601B8;
        c.s[3] -= D_001601B8;
        c.s[4] -= D_001601B8;
        c.s[5] -= D_001601BC;
        func_001F7090(&c, color, text, -1, col, font);
        if (w->flags & 0x400) {
            c.s[5] += c.s[7] + D_00160268 * 3;
            c.s[0] += D_001601BC;
            c.s[1] += D_001601BC;
            c.s[2] += D_001601B8;
            c.s[3] += D_001601B8;
            c.s[4] += D_001601B8;
            c.s[5] += D_001601BC;
            DisableGlobalStateFlag();
            func_001F7090(&c, 0x80000000L, text, -1, col, font);
            EnableGlobalStateFlag();
            c.s[0] -= D_001601BC;
            c.s[1] -= D_001601BC;
            c.s[2] -= D_001601B8;
            c.s[3] -= D_001601B8;
            c.s[4] -= D_001601B8;
            c.s[5] -= D_001601BC;
            func_001F7090(&c, color, text, -1, col, font);
            if (w->flags & 0x400) {
                w->scroll += (D_0013CAE0[0] & 1) ? 10 : 3;
                w->scroll %= (c.s[7] + D_00160268 * 3) * 16;
            }
        }
    }
    func_001F4398();
    return 2;
}
#endif /* NON_MATCHING */
