#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00239328/FUN_00239328.s", FUN_00239328);
#else
#include "types.h"

extern s32 D_001E6620[];
extern s32 D_001E6640[];
extern void func_00233980(s32, u64);
extern s32 func_00213260(s32);
extern s32 SubtractIntegerWithClamp(s32);
extern s64 func_001F44B8(s32);
extern f32 func_001FA6C0(s32);
extern void func_001F55D8(s32, s32, s32, s32, u64, s64, f32, f32, f32, f32);

void FUN_00239328(s32 k, f32 w, f32 h) {
    s32 t;
    s32 a;
    s32 alpha;
    f32 rx;
    f32 ry;
    f32 zero;
    f32 f;
    f32 hh;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;

    func_00233980(0x47, 0x32003);
    if (D_001E6620[k] != 0 || k == 0) {
        if (D_001E6620[k] != 0) {
            D_001E6620[k] += 2;
        }
        t = D_001E6620[k];
        if (k == 0) {
            if (t < 0x18) {
                t = 0x18;
            }
        }
        zero = 0.0f;
        rx = func_00213260(200);
        ry = func_00213260(200);
        alpha = 0x80 - SubtractIntegerWithClamp(t - 0x80);
        func_00233980(8, 0);
        alpha *= 2;
        if (alpha > 0x80) alpha = 0x80;
        func_00233980(0x42, ((u64)alpha << 32) | 0x68);
        func_001F55D8(rx + zero, ry + zero, w + rx, h + ry, 0x808080, func_001F44B8(0x1A), zero, zero, w, h);
        if (D_001E6620[k] >= 0x100) {
            D_001E6620[k] = 0;
        }
    }
    if (D_001E6620[k] == 0 && func_00213260(700) == 0) {
        D_001E6620[k] = 2;
    }
    func_00233980(8, 0);
    func_00233980(0x42, 0x8000000044ULL);
    if (k > 0) {
        if (D_001E6640[k] != 0) {
            D_001E6640[k] += 2;
            x0 = w;
            a = 0x100 - SubtractIntegerWithClamp(D_001E6640[k] - 0x100);
            if (a > 0x50) {
                a = 0x50;
            }
            f = -(func_001FA6C0(0x200 - D_001E6640[k]) * 0.03125f);
            hh = h + 16.0f;
            func_001F55D8(0, 0, x0, (s32)(hh * 1.5f), (a << 24) | 0x505050, func_001F44B8(0x1C), 0.0f, f, w, hh);
            if (D_001E6640[k] >= 0x200) {
                D_001E6640[k] = 0;
            }
        } else if (func_00213260(360) == 0) {
            D_001E6640[k] = 2;
        }
    }
    if (k == 6) {
        zero = 0.0f;
        func_001F55D8(0, 0, 0x40, 0x40, 0x80808080, func_001F44B8(0x19), zero, zero, w, h);
    }
}
#endif /* NON_MATCHING */
