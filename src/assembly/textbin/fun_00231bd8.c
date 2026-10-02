#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00231bd8/FUN_00231bd8.s", FUN_00231bd8);
#else
#include "types.h"
#include "sda.h"

typedef struct {
    u8 pad_0[0xD4];
    s32 xD4;
    s32 pad_D8;
    s32 xDC;
} SaveInfo;

typedef struct {
    u8 pad_0[0xC];
    s32 y;
} ScreenOfs;

extern u8 D_0013CDD0[];
extern SaveInfo D_0013D290;
extern ScreenOfs D_0013E500;
extern s32 D_0015ED84 __attribute__((sda));
extern s16 D_0015EE48 MACRO_ADDR;
extern s16 D_0015EE4A MACRO_ADDR;
struct PacketCursor {
    s32 *p;
};
extern struct PacketCursor D_00160F00_s __asm__("D_00160F00") MACRO_ADDR;
#define D_00160F00 (D_00160F00_s.p)

extern void func_0012F368(s32);
extern void func_001F3868(void);
extern void func_001F3958(void);
extern void func_001F4A58(s32);
extern void func_001F5450(s32, s32, s32, s32, s32, s32, s32, s32, u64, u64);
extern f32 func_001FA6C0(s32);
extern void func_001FB3D0(void);
extern s32 func_00204428(void);
extern void func_00208840(void);
extern void func_002093D8(void);
extern void func_002316E8(s32, s32, s32, s32, s32, u64, f32, f32, f32, f32);
extern void func_00231878(s32, s32, s32, u64 *, u64 *, u64 *);
extern void func_002335D0(void);
extern void func_00233630(void);
extern void func_002336A0(void);
extern void func_002337B0(s32);
extern void func_00233980(s32, u64);
extern void sceGsSyncV(s32);

void fun_00231bd8(s32 a0, s32 cur, s32 sel, s32 n, s32 skip) __asm__("FUN_00231bd8");

void fun_00231bd8(s32 a0, s32 cur, s32 sel, s32 n, s32 skip) {
    u64 tex0;
    u64 tex1;
    u64 tex2;
    s32 i;
    s32 a;
    s32 fade;
    f32 t;
    f32 t0;
    f32 t1;

    func_00231878(a0, cur, sel, &tex0, &tex1, &tex2);
    if (skip != 0) {
        func_0012F368(D_0015ED84);
        D_0015EE48 = 0;
        D_0015EE4A = 0;
    }
    sceGsSyncV(0);
    func_002335D0();
    for (i = 0; i < n && D_0013D290.xD4 < 3 && D_0013D290.xDC < 0; i++) {
        a = 0x80;
        func_001F3868();
        func_001FB3D0();
        func_00233980(1, (u64)0x8000 << 16);
        func_00233980(8, 0);
        D_00160F00[0] = 0x30000014;
        fade = i * 4;
        if (i <= 0x1F) {
            a = fade;
        }
        D_00160F00[1] = (s32)D_0013CDD0;
        D_00160F00[2] = 0;
        D_00160F00[3] = 0x50000014;
        D_00160F00 += 4;
        if (n - 0x10 < i) {
            a = (n - i) * 8;
        }
        t = func_001FA6C0(i % 600) * 0.0016666667f;
        if (cur == sel) {
            func_002316E8(0, D_0013E500.y - 0x20, 0x200, 0x40, (a << 24) | 0x808080, tex0, 0.0f, 4.0f,
                          t + 0.0f, t + 0.4f);
            func_001F5450(0, D_0013E500.y - 0x20, 0x200, 0x40, 0, 0, 0x200, 0x40, 0x80808080, tex1);
        } else {
            t0 = t + 0.0f;
            t1 = t + 0.4f;
            func_002316E8(0, D_0013E500.y - 0x2E, 0x200, 0x40, (a << 24) | 0x808080, tex0, 0.0f, 4.0f, t0,
                          t1);
            func_001F5450(0, D_0013E500.y - 0x2E, 0x200, 0x40, 0, 0, 0x200, 0x40, 0x80808080, tex1);
            if (i > 0x40) {
                if (i < 0x60) {
                    a = (i - 0x40) * 4;
                }
                func_002316E8(0, D_0013E500.y, 0x200, 0x40, (a << 24) | 0x808080, tex0, 0.0f, 4.0f, t0, t1);
                func_001F5450(0, D_0013E500.y, 0x200, 0x40, 0, 0, 0x200, 0x40, 0x80808080, tex2);
            }
        }
        func_002093D8();
        func_00208840();
        func_002337B0(1);
        sceGsSyncV(0);
        func_001F3958();
        func_002336A0();
        func_00233630();
        if (skip != 0) {
            if (func_00204428() == 0) {
                if (n < i + 0x14) {
                    n = i + 0x14;
                }
            } else {
                skip = 0;
            }
        }
    }
    func_001F4A58(2);
}
#endif /* NON_MATCHING */
