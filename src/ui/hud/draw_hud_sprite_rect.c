#include "types.h"
struct DmaTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct TagPtr { struct DmaTag *p; };
struct Screen { u8 pad[0x10]; s32 offx; s32 offy; };
struct HudState { u8 pad[0xC]; s32 z; };
extern struct TagPtr D_00160F00;
extern struct Screen D_0013E500;
extern struct HudState D_0019A3E8;
extern u64 func_001FFA10(s32);

void draw_hud_sprite_rect(s32 tex, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0, s32 u1, s32 v1, s32 alpha) __asm__("FUN_00200958");

void draw_hud_sprite_rect(s32 tex, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0, s32 u1, s32 v1, s32 alpha) {
    u64 *q;

    D_00160F00.p->w0 = 0x10000005;
    D_00160F00.p->addr = 0;
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0x50000005;
    q = (u64 *)++D_00160F00.p;
    q[0] = (u64)0xE800 << 47 | 0x8001;
    q[1] = 0x5353106;
    q[2] = func_001FFA10(tex);
    q[3] = 0x156;
    q[4] = (u64)alpha << 24 | 0x7F7F7F;
    q[5] = u0 | ((u64)v0 << 16);
    q[6] = (x0 + D_0013E500.offx - 8) | ((u64)(y0 + D_0013E500.offy - 8) << 16) | ((u64)D_0019A3E8.z << 32);
    q[7] = u1 | ((u64)v1 << 16);
    q[8] = (x1 + D_0013E500.offx - 8) | ((u64)(y1 + D_0013E500.offy - 8) << 16) | ((u64)D_0019A3E8.z << 32);
    q[9] = 0;
    D_00160F00.p += 5;
}

extern __typeof__(draw_hud_sprite_rect) func_00200958 __attribute__((alias("FUN_00200958")));
