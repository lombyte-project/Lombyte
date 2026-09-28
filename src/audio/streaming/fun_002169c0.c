#include "types.h"
struct Handle { u32 id; u8 pad4[6]; s16 state; };
struct Vec3s { s16 x; s16 y; s16 z; };
struct Game { u8 pad0[0x38]; struct Vec3s a; u8 pad3e[0x16]; struct Vec3s b; };
extern struct Game D_001516D0;
extern void func_00215970(s32, s32, s32);
void FUN_002169c0(u32 id, s64 arg) {
    struct Handle *h = (struct Handle *)(s32)arg;

    if (h != 0) {
        h->id = id;
        if (id != 0) {
            if (h->state == 1) {
                h->state = 2;
            }
        } else {
            func_00215970(D_001516D0.b.x, D_001516D0.b.z, D_001516D0.b.y);
        }
    }
}

extern __typeof__(FUN_002169c0) func_002169C0 __attribute__((alias("FUN_002169c0")));
