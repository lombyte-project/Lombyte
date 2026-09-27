#include "types.h"

struct Quad {
    f32 v[4][4];
    u32 col[4];
    u8 uv[0x20];
    u64 r0;
    u64 r1;
    u64 r2;
    u64 r3;
};

extern u64 D_00160580;
extern u8 D_001D97B0[];
extern s32 D_0015ED84;
extern f32 D_001D9A90[];
extern f32 D_001D9A50[][4];
extern s32 D_001604F0 __attribute__((sda));
extern void FUN_00233980(s32, u64);
extern void FUN_001f98d0(void *, void *, s32);
extern void FUN_001f9a68(f32 *, f32 *, f32);
extern void FUN_001f9a10(f32 *, f32 *, f32 *);
extern void func_001F7D30(struct Quad *, s32, s32);

void FUN_0022e8c8(void) {
    struct Quad q;
    f32 scale;
    s32 i;

    FUN_00233980(0x47, 0x31801);
    scale = 1.0f;
    q.r1 = D_00160580;
    q.r2 = 0xFF9000000260;
    q.r3 = 0x8000000044;
    q.r0 = 0;
    FUN_001f98d0(q.uv, D_001D97B0, 0x20);
    if ((u32)D_0015ED84 < 0x13) {
        scale = D_001D9A90[D_0015ED84];
    }
    for (i = 0; i < 4; i++) {
        q.col[i] = 0x80808080;
        FUN_001f9a68(q.v[i], D_001D9A50[i], scale);
        FUN_001f9a10(q.v[i], q.v[i], (f32 *)&D_001604F0);
    }
    func_001F7D30(&q, 0, 0);
    FUN_00233980(0x47, 0x5360B);
}

extern __typeof__(FUN_0022e8c8) func_0022E8C8 __attribute__((alias("FUN_0022e8c8")));
