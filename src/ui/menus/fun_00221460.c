#include "types.h"

struct OptItem { s32 text; u8 *value; s32 names[4]; };
struct OptMenu { u8 pad0[0x20]; s32 x; s32 height; u8 pad28[0xC]; struct OptItem *items; s32 selected; };

extern void func_00233980(s32, u64);
extern void func_001F4280(s32);
extern void func_001F4398(void);
extern s32 func_001FDD10(s32);
extern void func_001F6530(s32, s32, u64, s32, s32);
extern void func_001F6940(s32, s32, u64, s32, s32);

s32 FUN_00221460(struct OptMenu *m) {
    struct OptItem *p;
    struct OptItem *it;
    s32 n;
    s32 i;
    s32 step;
    s32 y;
    s32 color;

    func_00233980(0x47, 0x2004B);
    func_001F4280(0);
    n = 0;
    for (p = m->items; p->text != 0; p++) {
        n++;
    }
    step = m->height / (n + 1);
    p = m->items;
    y = step - 8;
    for (i = 0; m->items[i].text != 0; i++) {
        it = &m->items[i];
        if (i == m->selected) {
            color = 0x8020FFFF;
        } else {
            color = 0x80FFA888;
        }
        func_001F6530(0xC, y, color, func_001FDD10(it->text), -1);
        func_001F6940(m->x - 0xC, y, 0x80FFA888, func_001FDD10(it->names[*it->value]), -1);
        y += step;
    }
    func_001F4398();
    return 2;
}

extern __typeof__(FUN_00221460) func_00221460 __attribute__((alias("FUN_00221460")));
