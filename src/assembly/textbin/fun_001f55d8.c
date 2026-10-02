#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001f55d8/FUN_001f55d8.s", FUN_001f55d8);
#else
#include "types.h"
#include "qcopy.h"

struct DmaTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct TagPtr { struct DmaTag *p; };
struct ScreenOfs { u8 pad0[0x10]; s32 x; s32 y; };

extern struct TagPtr D_00160F00;
extern struct ScreenOfs D_0013E500;
extern char D_00160840[];
extern s32 func_001FA6D0(f32);

void FUN_001f55d8(s32 u, s32 v, s32 uw, s32 vh, s64 color, s64 extra, f32 x, f32 y, f32 w, f32 h)
{
    struct DmaTag *tag;
    u64 *q;
    s32 x0;
    s32 x1;
    s32 y0;
    s32 y1;
    s32 u1;
    s32 u0;
    s32 v1;

    x0 = func_001FA6D0(x * 16.0f) + D_0013E500.x - 8;
    x1 = func_001FA6D0((w + x) * 16.0f) + D_0013E500.x - 8;
    y0 = func_001FA6D0(y * 16.0f) + D_0013E500.y - 8;
    y1 = func_001FA6D0((y + h) * 16.0f) + D_0013E500.y - 8;
    u1 = (u + uw) << 4;
    u0 = u << 4;
    v1 = v + vh;
    D_00160F00.p->w0 = 0x10000007;
    D_00160F00.p->addr = 0;
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0x50000007;
    tag = D_00160F00.p;
    D_00160F00.p = tag + 1;
    qcopy(tag + 1, D_00160840);
    q = (u64 *)(tag + 2);
    D_00160F00.p = tag + 2;
    q[0] = extra;
    q[1] = 0x154;
    q[2] = color;
    q[3] = (v << 20) + u0;
    q[4] = x0 | ((u64)y0 << 16) | 0xFFFFF000000000;
    q[5] = (v << 20) + u1;
    q[6] = x1 | ((u64)y0 << 16) | 0xFFFFF000000000;
    q[7] = (v1 << 20) + u0;
    q[8] = x0 | ((u64)y1 << 16) | 0xFFFFF000000000;
    q[9] = (v1 << 20) + u1;
    q[10] = x1 | ((u64)y1 << 16) | 0xFFFFF000000000;
    q[11] = 0;
    D_00160F00.p = (struct DmaTag *)((u8 *)D_00160F00.p + 0x60);
}
#endif /* NON_MATCHING */
