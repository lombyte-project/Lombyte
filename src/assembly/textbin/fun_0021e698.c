#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021e698/FUN_0021e698.s", FUN_0021e698);
#else
#include "types.h"

struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};

struct Anchor {
    f32 x[2];
    f32 y;
    f32 z;
    u8 pad10[8];
    f32 side;
    f32 fwd;
};

struct Owner {
    u8 pad0[0x30];
    s32 flags;
    u8 pad34[4];
    f32 rot;
};

struct Link {
    struct Owner *owner;
    u8 pad4[8];
    s32 index;
};

struct Obj {
    u8 pad0[0x10];
    f32 x;
    f32 y;
    f32 z;
    u8 pad1C[0x2C];
    f32 rot;
    u8 pad4C[0x2C];
    struct Link *link;
};

struct Base {
    u8 pad0[0x140];
    struct Vec3 pos;
};

extern struct Base D_00186F40;
extern struct Anchor D_001E0408[];
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");

void FUN_0021e698(struct Obj *o) {
    struct Owner *w = o->link->owner;
    s32 i = o->link->index;
    struct Anchor *a;
    f32 side;
    f32 fwd;
    f32 c;
    f32 s;
    f32 nfwd;

    o->rot = w->rot;
    o->x = (&D_00186F40.pos)->x + ((w->flags & 1) ? D_001E0408[i].x[0] : D_001E0408[i].x[1]);
    o->y = (&D_00186F40.pos)->y + D_001E0408[i].y;
    o->z = (&D_00186F40.pos)->z + D_001E0408[i].z;
    fwd = D_001E0408[i].fwd;
    side = D_001E0408[i].side;
    c = fast_cos(o->rot);
    nfwd = -fwd;
    s = fast_sin(o->rot);
    o->x += nfwd * s + side * c;
    o->y += fwd * c + side * s;
}
#endif /* NON_MATCHING */
