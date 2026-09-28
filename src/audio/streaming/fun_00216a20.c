#include "types.h"
struct Handle { u32 id; u8 pad4[6]; s16 state; };
struct Vec3s { s16 x; s16 y; s16 z; };
struct Game { u8 pad0[0x38]; struct Vec3s a; u8 pad3e[0x16]; struct Vec3s b; };
extern struct Game D_001516D0;
extern void func_00215B68(s32, s32, s32);
void FUN_00216a20(u32 id, s64 arg) {
    struct Handle *h = (struct Handle *)(s32)arg;

    if (h != 0) {
        h->id = id;
        if (id != 0) {
            if (h->state == 1) {
                h->state = 2;
            }
        } else {
            func_00215B68(D_001516D0.a.x, D_001516D0.a.z, D_001516D0.a.y);
        }
    }
}

extern __typeof__(FUN_00216a20) func_00216A20 __attribute__((alias("FUN_00216a20")));
