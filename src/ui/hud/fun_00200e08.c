#include "types.h"
struct DmaTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct TagPtr { struct DmaTag *p; };
struct Screen { u8 pad[0x10]; s32 offx; s32 offy; };
extern struct TagPtr D_00160F00;
extern struct Screen D_0013E500;

void FUN_00200e08(s32 x0, s32 y0, s32 x1, s32 y1, u64 prim, s32 pixels) {
    u64 *q;

    D_00160F00.p->w0 = 0x10000003;
    D_00160F00.p->addr = 0;
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0x50000003;
    q = (u64 *)D_00160F00.p++;
    q[2] = (u64)0x8800 << 47 | 1;
    q[3] = 0x4410;
    q[4] = 0x46;
    q[5] = prim;
    if (pixels != 0) {
        q[6] = (x0 + D_0013E500.offx - 8) | ((u64)(y0 + D_0013E500.offy - 8) << 16) | (u64)0xFFFFF000 << 24;
        q[7] = (x1 + D_0013E500.offx - 8) | ((u64)(y1 + D_0013E500.offy - 8) << 16) | (u64)0xFFFFF000 << 24;
    } else {
        q[6] = ((x0 << 4) + D_0013E500.offx - 0x10) | ((u64)((y0 << 4) + D_0013E500.offy - 0x10) << 16) | (u64)0xFFFFF000 << 24;
        q[7] = ((x1 << 4) + D_0013E500.offx - 0x10) | ((u64)((y1 << 4) + D_0013E500.offy - 0x10) << 16) | (u64)0xFFFFF000 << 24;
    }
    D_00160F00.p += 3;
}

extern __typeof__(FUN_00200e08) func_00200E08 __attribute__((alias("FUN_00200e08")));
