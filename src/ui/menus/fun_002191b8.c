#include "types.h"
struct Obj { u8 pad0[0xC]; void (*release)(struct Obj *, s32); };
struct Owner { u8 pad0[0x44]; struct Obj *objs[14]; };
struct GameState { s32 state; struct Owner *owner; u8 pad8[0xC]; s32 unk14; s32 unk18; u8 pad1C[0xF4]; s32 progress; };
extern struct GameState D_001D5BF0;
extern s32 D_0015EE78;
extern s32 D_001D5D90[];
extern s32 FUN_00225530(s32);
void FUN_002191b8(void) {
    s32 i;
    s32 j;
    struct Obj *obj;

    if (D_001D5BF0.progress < 10) {
        return;
    }
    if (D_001D5BF0.owner != 0) {
        for (i = 0; i < 14; i++) {
            obj = D_001D5BF0.owner->objs[i];
            if (obj != 0 && obj->release != 0) {
                obj->release(obj, 0);
            }
        }
        D_001D5BF0.owner = 0;
    }
    D_0015EE78 = D_001D5BF0.unk18;
    for (j = 0; j < 14; j++) {
        D_001D5D90[j] = FUN_00225530(D_001D5D90[j]);
    }
    D_001D5BF0.state = 20;
    D_001D5BF0.unk14 = 2;
}

extern __typeof__(FUN_002191b8) func_002191B8 __attribute__((alias("FUN_002191b8")));
