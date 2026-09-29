#include "types.h"
#include "sda.h"
#include "qcopy.h"
#include "qzero.h"

struct M {
    s32 f0;
    s32 *f4;
    s32 *f8;
    u8 pad_C[4];
    s32 f10;
    u8 pad_14[4];
    s32 f18;
    u8 pad_1C[0xE0];
    s32 fFC;
    s32 f100;
    s32 f104;
    s32 f108;
    s32 f10C;
    u8 pad_110[0x14];
    s32 f124;
};
struct S { u8 pad_0[0x140]; float f140, f144, f148; };
struct P { u8 pad_0[0x10]; u8 f10; };
struct T { u8 pad_0[0x48]; struct P *tbl[1]; };
struct O2 {
    u8 pad_0[0x10];
    float f10, f14, f18;
    u8 pad_1C[8];
    struct T *f24;
    u8 pad_28[0xC];
    u16 f34;
    u8 pad_36[0xA];
    s32 f40, f44, f48;
    u8 pad_4C[0x28];
    s32 *f74;
};
extern struct M D_001D5BF0;
extern s32 D_001D45C8[];
extern u8 D_0019C150[];
extern u8 D_0019C160[];
extern s32 D_001940C0[];
extern s32 D_0015F438;
extern s32 D_00160F0C;
extern s32 D_0015EE78;
extern void FUN_0023a2c0();
extern struct S D_00186F40;
extern struct O2 *D_001D5D90[];
extern u8 D_001601C0 __attribute__((sda));
extern u8 D_001601D0 __attribute__((sda));
extern s32 FUN_001f9a68(s32, s32, f32);
extern void func_002337B0(s32);
extern s32 sceGsSyncV(s32);
extern s32 FUN_00225ac0();
extern void func_002335D0(void);
extern struct O2 *func_00225490();
extern s32 func_00212ED8();

void func_00225AC0__void() __asm__("FUN_00225ac0");
void FUN_00218f98(void) {
    s32 i;
    s32 a, b, c, d;
    D_001D5BF0.f0 = 2;
    D_001D5BF0.f4 = D_001D45C8; D_001D5BF0.f8 = D_001D45C8; D_001D5BF0.f124 = 0;
    FUN_001f9a68((s32)D_0019C150, (s32)&D_001601C0, 1.0f);
    qcopy(D_0019C150 - 0x10, &D_001601D0);
    qzero(D_0019C150 + 0x20);
    qzero(D_0019C160);
    func_002337B0(1);
    sceGsSyncV(0);
    D_0015F438++;
    a = D_001940C0[1] + 0xA0000;
    b = D_001940C0[2] + 0xA0000;
    c = a + 0x3C000;
    d = b + 0xE0000;
    D_001D5BF0.fFC = c + 0xC1000;
    D_001D5BF0.f100 = d + 0x11800;
    D_00160F0C = 0xA0000;
    D_001D5BF0.f104 = a;
    D_001D5BF0.f10 = b;
    D_001D5BF0.f108 = c;
    D_001D5BF0.f10C = d;
    func_00225AC0__void(1);
    func_002335D0();
    D_001D5BF0.f18 = D_0015EE78;
    if (D_001D5BF0.f4 != 0) {
        for (i = 0; i < 14; i++) {
            struct O2 *o = (struct O2 *)func_00225490(0x472);
            D_001D5D90[i] = o;
            if (o != 0) {
                s32 k;
                o->f34 &= 0xFFFD;
                D_001D5D90[i]->f74 = FUN_0023a2c0;
                D_001D5D90[i]->f10 = D_00186F40.f140;
                D_001D5D90[i]->f14 = D_00186F40.f144;
                D_001D5D90[i]->f18 = D_00186F40.f148;
                D_001D5D90[i]->f40 = 0;
                D_001D5D90[i]->f44 = 0;
                D_001D5D90[i]->f48 = 0;
                k = *(s32 *)((s32)D_001D5BF0.f4 + (i << 2));
                func_00212ED8(D_001D5D90[i], k, D_001D5D90[i]->f24->tbl[k]->f10 - 1);
            }
        }
    }
}

extern __typeof__(FUN_00218f98) func_00218F98 __attribute__((alias("FUN_00218f98")));
