#include "types.h"
struct LevelRec { u8 pad0[6]; s16 id; u8 pad8[2]; };
struct Level { u8 pad0[0x3C]; s32 cur; u8 pad40[8]; struct LevelRec *recs; };
struct Game { u8 pad0[0x40]; struct Level *level; };
struct Item { u8 pad0[0x44]; void *a; void *b; };
struct Descriptor { u8 pad0[0x10]; s16 w; s16 h; u8 pad14[0xC]; };
extern struct Game *D_001D5BF4[];
extern u8 D_0013D4C0[];
extern void PackImageDescriptor(struct Descriptor *, struct Item *);
extern void func_001F4280(s32);
extern void func_001F4398();
extern void func_001F7580(struct Descriptor *, u64, u8 *, s32);
extern u8 *func_001FDD10(s32);
extern void func_0020D330(void *, s32);
s32 FUN_0021e110(struct Item *item) {
    struct Descriptor desc;
    struct Level *level;

    level = D_001D5BF4[0]->level;
    if (D_0013D4C0[level->recs[level->cur].id] == 0) {
        return 0;
    }
    if (item->a != 0) {
        func_0020D330(item->a, 1);
        if (item->b != 0) {
            func_0020D330(item->b, 1);
        }
        return 8;
    }
    func_001F4280(0);
    PackImageDescriptor(&desc, item);
    desc.w = 0x10;
    desc.h = 3;
    func_001F7580(&desc, ((u64)0x80FF << 16) | 0xA888, func_001FDD10(0x4F4D), -1);
    func_001F4398();
    return 2;
}

extern __typeof__(FUN_0021e110) func_0021E110 __attribute__((alias("FUN_0021e110")));
