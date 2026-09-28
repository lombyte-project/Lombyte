#include "types.h"
struct Moby { u8 pad0[0x50]; u64 tag; u8 pad58[0x4E]; s16 oclass; };
struct Item { u8 pad0[0x50]; u64 tag; };
struct Owner { u8 pad0[0x44]; struct Item *items[24]; u8 held[24]; };
extern s32 D_001D5020[];
extern void func_0020D330(void *, s32);
s32 FUN_0021ddf8(struct Owner *o) {
    s32 i;
    u8 *first;

    for (i = 0; i < 24; i++) {
        if (o->items[i] == 0) {
            continue;
        }
        if (o->held[i] != 0) {
            continue;
        }
        if (D_001D5020[i] == 0) {
            continue;
        }
        if (i == 7 && ((struct Moby *)o->items[7])->oclass == 0x4A) {
            first = (u8 *)o->items[0];
            if (first[0x52] != first[0x53]) {
                continue;
            }
        }
        switch (i) {
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 10:
        case 11:
        case 12:
            if ((o->items[0]->tag & 0xFFFF0000) == 0x99990000
                && (u8)o->items[0]->tag >= 0x4D && (u8)o->items[0]->tag < 0x92) {
                continue;
            }
            break;
        }
        func_0020D330(o->items[i], 1);
    }
    return 4;
}

extern __typeof__(FUN_0021ddf8) func_0021DDF8 __attribute__((alias("FUN_0021ddf8")));
