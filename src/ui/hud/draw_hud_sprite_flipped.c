#include "types.h"

struct DmaTag {
    u32 w0;
    u32 addr;
    u32 w2;
    u32 w3;
};

struct TagPtr {
    struct DmaTag *p;
};

struct TexInfo {
    u8 pad0[6];
    u8 wlog;
    u8 hlog;
};

struct TexBank {
    u8 pad0[0xC];
    s32 z;
    u8 pad10[0x10];
    s16 *map;
    struct TexInfo *tex;
};

struct ScreenOfs {
    u8 pad0[0x10];
    s32 x;
    s32 y;
};

extern struct TagPtr D_00160F00;
extern struct TexBank D_0019A3E8;
extern struct ScreenOfs D_0013E500;
extern u64 func_001FFA10(s32);

void draw_hud_sprite_flipped(s32 id, s32 x, s32 y, s32 w, s32 h, s32 alpha) __asm__("FUN_001ffe18");

void draw_hud_sprite_flipped(s32 id, s32 x, s32 y, s32 w, s32 h, s32 alpha) {
    struct TexInfo *t;
    struct DmaTag *tag;
    u64 *q;
    s32 tw;
    s32 th;

    t = &D_0019A3E8.tex[D_0019A3E8.map[id * 2 + 1]];
    tw = 1 << t->wlog;
    th = 1 << t->hlog;
    D_00160F00.p->w0 = 0x10000007;
    D_00160F00.p->addr = 0;
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0x50000007;
    tag = D_00160F00.p;
    q = (u64 *)(tag + 1);
    D_00160F00.p = tag + 1;
    q[0] = 0xB400000000008001;
    q[1] = 0x53535353106;
    q[2] = func_001FFA10(id);
    q[3] = 0x154;
    q[4] = ((u64)alpha << 24) | 0x7F7F7F;
    q[5] = tw << 4;
    q[6] = (((x << 4) + D_0013E500.x) - 8) | ((u64)((((y + h) << 4) + D_0013E500.y) - 8) << 16) |
           ((u64)D_0019A3E8.z << 32);
    q[7] = (th << 20) + (tw << 4);
    q[8] = (((x << 4) + D_0013E500.x) - 8) | ((u64)(((y << 4) + D_0013E500.y) - 8) << 16) |
           ((u64)D_0019A3E8.z << 32);
    q[9] = 0;
    q[10] = ((((x + w) << 4) + D_0013E500.x) - 8) | ((u64)((((y + h) << 4) + D_0013E500.y) - 8) << 16) |
            ((u64)D_0019A3E8.z << 32);
    q[11] = th << 20;
    q[12] = ((((x + w) << 4) + D_0013E500.x) - 8) | ((u64)(((y << 4) + D_0013E500.y) - 8) << 16) |
            ((u64)D_0019A3E8.z << 32);
    q[13] = 0;
    D_00160F00.p = (struct DmaTag *)((u8 *)D_00160F00.p + 0x70);
}

