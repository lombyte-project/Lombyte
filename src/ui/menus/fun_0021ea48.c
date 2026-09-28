#include "types.h"
struct Owner { u8 pad0[0x44]; void *child; };
struct Moby {
    u8 pad0[0x10]; f32 x; f32 y; f32 z; u8 pad1c[0x18]; s16 unk34; u8 pad36[0xE];
    f32 unk44; u8 pad48[0x2C]; void (*update)(struct Moby *); struct Owner **vars;
};
struct Cam { u8 pad0[0x140]; f32 x; f32 y; f32 z; };
extern struct Cam D_00186F40;
extern struct Moby *func_00225490(s32);
extern void FUN_0021f120(struct Moby *);
s32 FUN_0021ea48(struct Owner *owner) {
    struct Moby *m = func_00225490(0x46E);

    if (m != 0) {
        owner->child = m;
        m->unk34 = 0;
        m->x = D_00186F40.x + 8.0f;
        m->y = D_00186F40.y + 0.5f;
        m->z = D_00186F40.z - 0.1f;
        m->unk44 = -1.9f;
        m->update = FUN_0021f120;
        *m->vars = owner;
    }
    return 0;
}

extern __typeof__(FUN_0021ea48) func_0021EA48 __attribute__((alias("FUN_0021ea48")));
