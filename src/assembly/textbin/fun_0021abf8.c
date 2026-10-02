#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021abf8/FUN_0021abf8.s", FUN_0021abf8);
#else
#include "types.h"
#include "sda.h"

typedef struct {
    s16 type;
    s16 action;
    union {
        s32 i;
        struct {
            u16 lo;
            s16 hi;
        } h;
    } param;
    s16 f8;
    u16 timer;
} MenuItem;

typedef struct {
    u8 pad0[0x14];
    s32 sound;
    u8 pad18[0x18];
    s32 flags;
    MenuItem *items;
    s32 prev;
    s32 next;
    s32 sel;
} Menu;

typedef struct {
    u8 pad0[0x38];
    s32 back;
    u8 pad3C[4];
    Menu *focus;
    u8 pad44[0x3C];
    s32 link;
} Screen;

typedef struct {
    s32 unk0;
    Screen *screen;
    s32 next;
    s32 mode;
    u8 pad10[0xD4];
    s32 unkE4;
    u8 padE8[4];
    s32 unkEC;
    Screen *unkF0;
    s32 unkF4;
    u8 padF8[0x2C];
    s32 unk124;
} MenuState;

typedef struct {
    u8 pad0[0x1B4];
    s32 held;
    u8 pad1B8[0xC];
    s32 pressed;
} Pad;

extern Pad D_0013C940;
extern s32 D_0015ED84 __attribute__((sda));
extern s32 D_0015ED88 __attribute__((sda));
extern s32 D_0015EEB0;
extern s32 D_0015EEB4;
extern s32 D_001601B4 __attribute__((sda));
extern s32 *D_001601E0 __attribute__((sda));
extern s32 D_00199478[];
extern s32 D_001A0314[];
extern MenuState D_001D5BF0;

extern s32 func_001F96F8(s32);
extern void func_001FBAB8(s32, s32);
extern s32 func_0022DA68(s32, s32, s32);

s32 FUN_0021abf8(Menu *m) __asm__("FUN_0021abf8");

s32 FUN_0021abf8(Menu *m)
{
    s32 match;
    s32 i;
    s32 n;
    s32 old;
    s32 flags;
    s32 buttons;
    s16 t;
    s16 code;
    MenuItem *items;
    MenuItem *it;
    s32 sel;
    s32 act;

    match = D_001D5BF0.screen->focus == m;
    for (i = 0; m->items[i].type != 0; i++) {
        if (match && m->sel == i) {
            m->items[i].timer = m->items[i].timer + 1;
        } else {
            t = (s16)m->items[i].timer;
            if (func_001F96F8(D_001601B4) < t) {
                m->items[i].timer = func_001F96F8(D_001601B4);
            }
            m->items[i].timer = (s16)m->items[i].timer > 0 ? m->items[i].timer - 1 : 0;
        }
    }
    if (!match) {
        return 0;
    }
    if (D_0013C940.pressed & 0xD00) {
        if (m->flags & 0x20) {
            D_001A0314[0] = D_0015ED84;
        }
        return -1;
    }
    if (D_0013C940.pressed & 0x10) {
        if (m->flags & 0x20) {
            D_001A0314[0] = D_0015ED84;
        }
        if (D_001D5BF0.screen->back != 0) {
            D_001D5BF0.next = D_001D5BF0.screen->back;
        } else if (D_001D5BF0.unk124 == 0) {
            return -1;
        }
    }
    if (D_0013C940.pressed & 0x40) {
        switch (m->items[m->sel].action) {
        case 0:
            break;
        case 1:
        case 3:
            D_001D5BF0.next = m->items[m->sel].param.i;
            break;
        case 4:
            func_0022DA68(0, 0x11, m->sound);
            if (D_0015EEB0 == 1 || D_0015EEB0 == 0x10) {
                D_001D5BF0.next = m->items[m->sel].param.i;
            } else {
                D_0015EEB4 |= 2;
                func_001FBAB8(3, m->items[m->sel].param.i);
            }
            break;
        case 5:
            func_0022DA68(0, 0x11, m->sound);
            if (D_0015EEB0 == 1 || D_0015EEB0 == 0x10) {
                D_001D5BF0.next = m->items[m->sel].param.i;
            } else {
                D_0015EEB4 |= 4;
                func_001FBAB8(3, m->items[m->sel].param.i);
            }
            break;
        case 6:
            code = m->items[m->sel].param.h.hi;
            if (code != 0) {
                D_001D5BF0.unkEC = D_00199478[code];
            }
            D_001D5BF0.mode = 5;
            D_001D5BF0.unkF0 = D_001D5BF0.screen;
            D_001D5BF0.unkF4 = 0;
            D_001D5BF0.unkE4 = m->items[m->sel].param.h.lo;
            func_0022DA68(0, 0x11, m->sound);
            return 0;
        case 7:
            D_001D5BF0.unkF4 = 2;
            D_001D5BF0.unkF0 = D_001D5BF0.screen;
            D_001D5BF0.mode = 3;
            D_001D5BF0.unkE4 = m->items[m->sel].param.i;
            func_0022DA68(0, 0x11, m->sound);
            return 0;
        case 8:
            D_001D5BF0.unkF4 = 2;
            D_001D5BF0.unkF0 = D_001D5BF0.screen;
            D_001D5BF0.mode = 4;
            D_001D5BF0.unkE4 = m->items[m->sel].param.i;
            func_0022DA68(0, 0x11, m->sound);
            return 0;
        case 10:
            D_001D5BF0.unkF4 = 2;
            D_001D5BF0.unkF0 = D_001D5BF0.screen;
            D_001D5BF0.mode = 6;
            D_001D5BF0.unkE4 = m->items[m->sel].param.i;
            func_0022DA68(0, 0x11, m->sound);
            return 0;
        case 11:
            D_001D5BF0.unkF0 = D_001D5BF0.screen;
            D_001D5BF0.unkF4 = 2;
            D_001D5BF0.mode = 7;
            func_0022DA68(0, 0x11, m->sound);
            return 0;
        case 9:
            D_0015ED88 = m->items[m->sel].param.i;
            return 0;
        case 2:
            func_0022DA68(2, 0x11, m->sound);
            break;
        }
    }
    n = 0;
    old = m->sel;
    flags = m->flags;
    if (m->items[0].type != 0) {
        do {
            it = &m->items[n];
            n++;
        } while (it[1].type != 0);
    }
    if (flags & 1) {
        buttons = D_0013C940.held;
    } else {
        buttons = D_0013C940.pressed;
    }
    if ((buttons & 0x1000) || ((flags & 0x100) && (buttons & 4))) {
        if (m->sel != 0) {
            m->sel--;
        } else if (flags & 0x1000) {
            m->sel = n - 1;
        } else {
            D_001D5BF0.screen->link = m->prev;
        }
    }
    if ((buttons & 0x4000) || ((m->flags & 0x100) && (buttons & 8))) {
        if (m->items[m->sel + 1].type != 0 && m->items[m->sel + 1].action != 0) {
            m->sel++;
        } else if (m->flags & 0x1000) {
            m->sel = 0;
        } else {
            D_001D5BF0.screen->link = m->next;
        }
    }
    if (m->sel != old || D_001D5BF0.screen->link != 0) {
        func_0022DA68(1, 0x11, m->sound);
        if (m->flags & 0x20) {
            D_001A0314[0] = D_001601E0[m->sel];
        }
    }
    return 0;
}
#endif /* NON_MATCHING */
