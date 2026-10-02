#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021b858/FUN_0021b858.s", FUN_0021b858);
#else
#include "types.h"

typedef struct Widget Widget;

typedef struct {
    u8 pad0[6];
    s16 item;
    u8 pad8[2];
} MenuEntry;

struct Widget {
    u8 pad0[0x14];
    s32 sound;
    u8 pad18[0x18];
    s32 flags;
    u8 pad34[0x8];
    s32 cursor;
    s32 rows;
    s32 cols;
    MenuEntry *entries;
    Widget *up;
    Widget *down;
    Widget *left;
    Widget *right;
};

typedef struct {
    u8 pad0[0x38];
    s32 back;
    u8 pad3C[0x4];
    Widget *focus;
    u8 pad44[0x3C];
    Widget *next;
} MenuPage;

typedef struct {
    s32 state;
    MenuPage *page;
    s32 result;
    u8 padC[0x24];
    s32 equip[0x3D];
    s32 busy;
    u8 pad128[0xC];
    s32 hideA;
    s32 hideB;
} MenuState;

typedef struct {
    u8 pad0[0x1C4];
    s32 pressed;
} PadState;

typedef struct {
    u8 pad0[0x8];
    s32 slot;
    u8 padC[0x40];
} ItemInfo;

typedef struct {
    u8 pad0[0x1FF5];
    u8 flag;
    u8 pad1FF6;
    u8 count;
} GameSave;

extern PadState D_0013C940;
extern u8 D_0013D4C0[];
extern GameSave D_0013F350;
extern ItemInfo D_001863D0[];
extern MenuState D_001D5BF0;
extern s32 func_001E9468();
extern s32 func_0022DA68();

s32 FUN_0021b858(Widget *w) {
    s32 width;
    s32 height;
    s32 cursor;
    s32 row;
    s32 col;
    s32 column;
    s32 skip;
    s32 nrows;
    s32 back;
    s32 total;
    s32 m;
    s32 n;
    Widget *next;
    MenuEntry *entry;
    s32 slot;

    if (D_001D5BF0.page->focus != w) {
        return 0;
    }
    if ((D_0013C940.pressed & 0xD00) && D_001D5BF0.busy == 0) {
        return 1;
    }
    if (D_0013C940.pressed & 0x10) {
        back = D_001D5BF0.page->back;
        if (back != 0) {
            D_001D5BF0.result = back;
        } else if (D_001D5BF0.busy == 0) {
            return -1;
        }
    }

    width = w->cols;
    cursor = w->cursor;
    height = w->rows;
    row = cursor / width;
    col = cursor % width;
    column = col;

    if (D_0013C940.pressed & 0x1000) {
        if (row != 0) {
            w->cursor = cursor - width;
        } else if (w->up != NULL) {
            next = w;
            do {
                next = next->up;
                nrows = next->rows;
                n = next->cols;
                skip = 0;
                if (D_001D5BF0.hideA != 0 && (next->flags & 8)) {
                    skip = 1;
                }
                if (D_001D5BF0.hideB != 0 && (next->flags & 4)) {
                    skip = 1;
                }
            } while (skip);
            D_001D5BF0.page->next = next;
            if (n == 5 && w->cols == 3) {
                column++;
            }
            if (n == 3 && w->cols == 5) {
                column--;
                if (column > 2) {
                    column = 2;
                } else if (column < 0) {
                    column = 0;
                }
            }
            next->cursor = (nrows - 1) * n + (column < n - 1 ? column : n - 1);
        } else if (!(w->flags & 0x8000)) {
            w->cursor = width * (height - 1) + cursor;
        }
    }

    if (D_0013C940.pressed & 0x4000) {
        if (row + 1 < height) {
            w->cursor = w->cursor + width;
        } else if (w->down != NULL) {
            next = w;
            do {
                next = next->down;
                m = next->cols;
                skip = 0;
                if (D_001D5BF0.hideA != 0 && (next->flags & 8)) {
                    skip = 1;
                }
                if (D_001D5BF0.hideB != 0 && (next->flags & 4)) {
                    skip = 1;
                }
            } while (skip);
            D_001D5BF0.page->next = next;
            if (m == 5 && w->cols == 3) {
                column++;
            }
            if (m == 3 && w->cols == 5) {
                column--;
                if (column > 2) {
                    column = 2;
                } else if (column < 0) {
                    column = 0;
                }
            }
            next->cursor = column < m - 1 ? column : m - 1;
        } else if (!(w->flags & 0x8000)) {
            w->cursor = w->cursor - w->cols * (w->rows - 1);
        }
    }

    if (D_0013C940.pressed & 0x8000) {
        if (column != 0) {
            w->cursor = w->cursor - 1;
        } else if (w->left != NULL) {
            D_001D5BF0.page->next = w->left;
        } else if (!(w->flags & 0x8000)) {
            w->cursor += w->cols - 1;
        }
    }

    if (D_0013C940.pressed & 0x2000) {
        if (column + 1 < width) {
            w->cursor = w->cursor + 1;
        } else if (w->right != NULL) {
            D_001D5BF0.page->next = w->right;
        } else if (!(w->flags & 0x8000)) {
            w->cursor -= w->cols - 1;
        }
    }

    if (w->cursor != cursor || D_001D5BF0.page->next != NULL) {
        func_0022DA68(1, 0x11, w->sound);
    }

    if ((D_0013C940.pressed & 0x40) && ((w->flags ^ 1) & 1)) {
        entry = &w->entries[w->cursor];
        if (entry->item != 0 && D_0013D4C0[entry->item] != 0) {
            slot = D_001863D0[entry->item].slot;
            func_0022DA68(0, 0x11, w->sound);
            if (D_001D5BF0.equip[slot] == entry->item && slot != 0 && slot != 3) {
                D_001D5BF0.equip[slot] = 0;
            } else if (entry->item == 0x18) {
                if (func_001E9468(0x18) != 0 && D_001D5BF0.state == 3) {
                    n = func_001E9468(0x18);
                    D_0013F350.flag = 1;
                    total = D_0013F350.count + n;
                    if (total > 6) {
                        total = 6;
                    }
                    D_0013F350.count = total;
                }
            } else {
                D_001D5BF0.equip[slot] = entry->item;
            }
        } else {
            func_0022DA68(2, 0x11, w->sound);
        }
    }
    return 0;
}
#endif /* NON_MATCHING */
