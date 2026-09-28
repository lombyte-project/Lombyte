#include "types.h"
struct GifTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct TagPtr { struct GifTag *p; };
struct Disp { u8 pad0[0x230]; s32 a; s32 b; s32 c; };
extern struct TagPtr D_00160F00;
extern u8 D_0013CFC0[];
extern u8 D_0013CF10[];
extern struct Disp D_0018CD00;
extern void func_00233980(s32, u64);
void reset_gs_registers(void) __asm__("FUN_001f3868");

void reset_gs_registers(void) {
    struct GifTag *p;

    D_00160F00.p->w0 = 0x30000013;
    D_00160F00.p->addr = (u32)D_0013CFC0;
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0x50000013;
    p = D_00160F00.p;
    D_00160F00.p = p + 1;
    p[1].w0 = 0x3000000B;
    D_00160F00.p->addr = (u32)D_0013CF10;
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0x5000000B;
    D_00160F00.p++;
    func_00233980(0x3D, D_0018CD00.a | ((u64)D_0018CD00.b << 8) | ((u64)D_0018CD00.c << 16));
}

extern __typeof__(reset_gs_registers) func_001F3868 __attribute__((alias("FUN_001f3868")));
