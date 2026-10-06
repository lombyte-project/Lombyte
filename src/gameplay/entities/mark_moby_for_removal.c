#include "types.h"
struct Obj {
    u8 pad0[0x20];
    u8 state;
    u8 pad21[0x17];
    s64 time;
};
extern struct Obj *D_0015FF1C;
extern s32 D_0015F60C;
extern void func_0020DC20(struct Obj *, u32);
void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

void mark_moby_for_removal(struct Obj *obj) {
    if (obj < D_0015FF1C) {
        obj->state = 0xFD;
    } else {
        obj->state = 0xFE;
    }
    obj->time = D_0015F60C + 2;
    func_0020DC20(obj, 0x80807F7F);
}

extern __typeof__(mark_moby_for_removal) func_0020C828 __attribute__((alias("FUN_0020c828")));
