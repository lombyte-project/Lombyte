#include "types.h"
#include "qcopy.h"

struct DmaTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct TagPtr { struct DmaTag *p; };

typedef struct {
    u8 pad0[0x190];
    u8 v190[0x10];
    u8 v1A0[0x10];
    u8 pad1B0[0x60];
    f32 f210;
    u8 pad214[0x14];
    f32 f228;
    f32 f22C;
} Camera;

extern struct TagPtr D_00160F00;
extern u16 D_0010E800[];
extern u8 D_0010E810[];
extern s32 D_0015F620;
extern f32 D_0015F348 __attribute__((sda));
extern u8 D_00187080[];
extern Camera D_0018CD00;
extern void FUN_001f9ff8(f32 (*)[4], f32);
extern void FUN_001f9a68(f32 *, u8 *, f32);
extern void FUN_001fa378(void *, u8 *, f32 (*)[4]);
extern void func_00233830(u8 *, s32);
extern void func_00233C90(void);

void FUN_001f76a0(void)
{
    f32 m[4][4];
    struct DmaTag *base;
    u8 *p;

    FUN_001f9ff8(m, 1024.0f);
    FUN_001f9a68(m[3], D_00187080, -1024.0f);
    m[3][3] = 1.0f;
    if (D_0015F620 != 7) {
        func_00233830(D_0010E810, D_0010E800[0]);
        D_0015F620 = 7;
    }
    D_00160F00.p->w0 = 0x10000000;
    D_00160F00.p->addr = 0;
    D_00160F00.p->w2 = 0x11000000;
    D_00160F00.p->w3 = 0x1000404;
    base = D_00160F00.p;
    base[1].w0 = 0;
    base[1].addr = 0;
    base[1].w2 = 0;
    base[1].w3 = 0x6C0C43A4;
    p = (u8 *)(base + 2);
    FUN_001fa378(p, D_00187080 - 0x100, m);
    *(f32 *)(p + 0x38) += D_0015F348;
    p = (u8 *)(base + 6);
    FUN_001fa378(p, D_00187080 - 0x80, m);
    *(f32 *)(p + 0x38) += D_0015F348;
    base[10].w0 = 0x8000;
    base[10].addr = 0x303EC000;
    base[10].w2 = 0x412;
    *(f32 *)&base[10].w3 = D_0018CD00.f210;
    p = (u8 *)(base + 11);
    qcopy(p, D_0018CD00.v190);
    p = (u8 *)(base + 12);
    qcopy(p, D_0018CD00.v1A0);
    *(f32 *)&base[13].w0 = D_0018CD00.f22C;
    *(f32 *)&base[13].addr = D_0018CD00.f228;
    base[13].w2 = 0;
    base[13].w3 = 0;
    base[14].w0 = 0x3000000;
    base[14].addr = 0x20001D2;
    base[14].w2 = 0x15000000;
    base[14].w3 = 0;
    p = (u8 *)(base + 15);
    D_00160F00.p->w0 |= (((u8 *)p - (u8 *)D_00160F00.p) >> 4) - 1;
    D_00160F00.p = (struct DmaTag *)p;
    func_00233C90();
}

extern __typeof__(FUN_001f76a0) func_001F76A0 __attribute__((alias("FUN_001f76a0")));
