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

struct Vec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

struct ScreenOfs {
    u8 pad0[0x10];
    s32 x;
    s32 y;
};

struct TexBank {
    u8 pad0[0xC];
    s32 z;
};

extern struct TagPtr D_00160F00;
extern struct ScreenOfs D_0013E500;
extern struct TexBank D_0019A3E8;
extern void FUN_001f9a10(void *, void *, void *);
extern void FUN_001f9a28(void *, void *, void *);
extern f32 func_001F9DC8(f32);
extern f32 func_001F9DE0(f32);

void FUN_00200600(s32 w, s32 h, s64 buf, f32 x, f32 y, f32 cx, f32 cy, f32 ang) {
    struct Vec4 sz;
    struct Vec4 off;
    struct Vec4 pos;
    struct Vec4 p0;
    struct Vec4 p1;
    struct Vec4 p2;
    struct Vec4 p3;
    struct DmaTag *tag;
    u64 *q;

    pos.x = x;
    pos.y = y;
    sz.x = cy * func_001F9DE0(ang);
    sz.y = cy * func_001F9DC8(ang);
    off.x = -cx * func_001F9DC8(ang);
    off.y = cx * func_001F9DE0(ang);
    FUN_001f9a10(&p0, &pos, &sz);
    FUN_001f9a28(&p0, &p0, &off);
    FUN_001f9a10(&p1, &pos, &sz);
    FUN_001f9a10(&p1, &p1, &off);
    FUN_001f9a28(&p2, &pos, &sz);
    FUN_001f9a28(&p2, &p2, &off);
    FUN_001f9a28(&p3, &pos, &sz);
    FUN_001f9a10(&p3, &p3, &off);
    D_00160F00.p->w0 = 0x10000007;
    D_00160F00.p->addr = 0;
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0x50000007;
    tag = D_00160F00.p;
    q = (u64 *)(tag + 1);
    D_00160F00.p = tag + 1;
    q[0] = 0xB400000000008001;
    q[1] = 0x53535353106;
    q[2] = buf;
    q[3] = 0x154;
    q[4] = 0x807F7F7F;
    q[5] = w << 4;
    q[6] = (((s32)p0.x + D_0013E500.x) - 8) | ((u64)(((s32)p0.y + D_0013E500.y) - 8) << 16) |
           ((u64)D_0019A3E8.z << 32);
    q[7] = (h << 20) + (w << 4);
    q[8] = (((s32)p1.x + D_0013E500.x) - 8) | ((u64)(((s32)p1.y + D_0013E500.y) - 8) << 16) |
           ((u64)D_0019A3E8.z << 32);
    q[9] = 0;
    q[10] = (((s32)p2.x + D_0013E500.x) - 8) | ((u64)(((s32)p2.y + D_0013E500.y) - 8) << 16) |
            ((u64)D_0019A3E8.z << 32);
    q[11] = h << 20;
    q[12] = (((s32)p3.x + D_0013E500.x) - 8) | ((u64)(((s32)p3.y + D_0013E500.y) - 8) << 16) |
            ((u64)D_0019A3E8.z << 32);
    q[13] = 0;
    D_00160F00.p = (struct DmaTag *)((u8 *)D_00160F00.p + 0x70);
}

extern __typeof__(FUN_00200600) func_00200600 __attribute__((alias("FUN_00200600")));
