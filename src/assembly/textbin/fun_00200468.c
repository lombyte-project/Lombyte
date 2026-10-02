#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00200468/FUN_00200468.s", FUN_00200468);
#else
#include "types.h"

struct DmaTag {
    u32 w0;
    u32 addr;
    u32 w2;
    u32 w3;
    s64 unk10;
    s64 unk18;
    s64 unk20;
    s64 unk28;
    s64 unk30;
    s64 unk38;
    s64 unk40;
    s64 unk48;
};

struct TagPtr {
    struct DmaTag *p;
};

struct TexBank {
    u8 pad0[0xC];
    s32 z;
};

struct ScreenOfs {
    u8 pad0[0x10];
    s32 x;
    s32 y;
};

extern struct TagPtr D_00160F00;
extern struct ScreenOfs D_0013E500;
extern struct TexBank D_0019A3E8;

void FUN_00200468(s32 tex0, s32 x, s32 y, s32 wlog, s32 hlog, s32 w, s32 h, s32 u, s32 v,
                  s32 alpha) {
    struct DmaTag *p;
    s64 *q;

    D_00160F00.p->w0 = 0x10000005;
    D_00160F00.p->addr = 0;
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0x50000005;
    p = D_00160F00.p;
    q = (s64 *)((u8 *)p + 0x10);
    D_00160F00.p = (struct DmaTag *)q;
    p->unk10 = 0x7400000000008001;
    q[2] = tex0;
    q[1] = 0x5353106;
    q[4] = ((s64)alpha << 24) | 0x7F7F7F;
    q[3] = 0x156;
    q[5] = u | ((s64)v << 16);
    q[6] = (x + D_0013E500.x - 8) | ((s64)(y + D_0013E500.y - 8) << 16) |
           ((u64)D_0019A3E8.z << 32);
    q[7] = (u + (1 << (wlog + 4))) | ((s64)(v + (1 << (hlog + 4))) << 16);
    q[8] = ((x + w) + D_0013E500.x - 8) | ((s64)((y + h) + D_0013E500.y - 8) << 16) |
           ((u64)D_0019A3E8.z << 32);
    q[9] = 0;
    D_00160F00.p = (struct DmaTag *)((u8 *)D_00160F00.p + 0x50);
}
#endif /* NON_MATCHING */
