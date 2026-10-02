#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/draw_cheats_menu/FUN_00221030.s", FUN_00221030);
#else
#include "types.h"

struct Box {
    s16 v[12];
};

struct Cheat {
    s32 name;
    u8 *flag;
    s32 on;
    s32 off;
    s32 pad10;
};

struct CheatMenu {
    u8 pad0[0x20];
    s32 w;
    s32 h;
    u8 pad28[8];
    s32 flags;
    struct Cheat *list;
    s32 sel;
};

extern void func_00233980(s32, u64);
extern void func_001F4280(s32);
extern void func_001F4398(void);
extern void func_001153FC(void *, s32, u32);
extern s32 func_001FDD10(s32);
extern void func_001F7580(struct Box *, u64, s32, s32);
extern void func_001F6530(s32, s32, u64, s32, s32);
extern void func_001F6940(s32, s32, u64, s32, s32);

s32 draw_cheats_menu(struct CheatMenu *m) __asm__("FUN_00221030");

s32 draw_cheats_menu(struct CheatMenu *m) {
    struct Box tmp;
    s16 box[12];
    struct Cheat *c;
    struct Cheat *p;
    s32 n;
    s32 i;
    s32 step;
    s32 y;
    s32 color;
    s32 on;
    s32 j;

    func_00233980(0x47, 0x2004B);
    func_001F4280(0);
    if ((m->flags & 1) && m->list->name == 0) {
        func_001153FC(box, 0, sizeof(box));
        box[1] = m->h + 1;
        box[3] = m->w + 1;
        box[4] = m->w >> 1;
        box[0] = 1;
        box[2] = 1;
        box[5] = m->h / 3;
        box[8] = 16;
        box[9] = 1;

        tmp = *(struct Box *)box;
        func_001F7580(&tmp, 0x80FFA888, func_001FDD10(0x4FC0), -1);
    }
    n = 0;
    while (m->list[n].name != 0) {
        n++;
    }
    step = m->h / (n + 1);
    y = step - 8;
    i = 0;
    j = 0;
    if (m->list[0].name != 0) {
        do {
            c = &m->list[j];
            color = i == m->sel ? 0x8020FFFF : 0x80FFA888;
            on = 0;
            if (c->flag != 0) {
                on = *c->flag;
            }
            func_001F6530(0xC, y, color, func_001FDD10(c->name), -1);
            func_001F6940(m->w - 0xC, y, 0x80FFA888, func_001FDD10(on ? c->on : c->off), -1);
            y += step;
            j++;
            i++;
        } while (m->list[i].name != 0);
    }
    func_001F4398();
    return 2;
}
#endif /* NON_MATCHING */
