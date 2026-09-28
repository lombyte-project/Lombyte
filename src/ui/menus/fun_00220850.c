#include "types.h"

struct Screen {
    u8 pad0[0x160];
    s16 x;
    s16 y;
};

struct Hud {
    u8 pad0[0x228];
    s32 state;
    u8 pad22C[0x2C];
    u64 tex0;
    u64 tex1;
    u64 tex2;
};

extern struct Screen D_00151780;
extern volatile s32 D_0015F438;
extern struct Hud D_001A00F0;
extern s32 D_001DDF68[];
extern void FUN_00200468(u64, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_00233980(s32, u64);

s32 FUN_00220850(void) {
    s32 st = D_001A00F0.state;
    s32 size;
    s32 off;
    s32 t;
    s32 ph;

    if (st < 0) {
        return 0;
    }
    if (st == 6 || st == 13 || st == 17) {
        func_00233980(0x47, 0);
        func_00233980(8, 5);
        FUN_00200468(D_001A00F0.tex0, 0, 0, 7, 7, D_00151780.x << 4, D_00151780.y << 4, 0, 0, 0x80);
        func_00233980(0x47, 0x360B);
        if (D_0015F438 % 60 < 40) {
            FUN_00200468(D_001A00F0.tex1, 0, 0, 7, 7, D_00151780.x << 4, D_00151780.y << 4, 0, 0, 0x80);
        }
        if (D_0015F438 % 150 < 90) {
            FUN_00200468(D_001A00F0.tex2, 0, 0, 7, 7, D_00151780.x << 4, D_00151780.y << 4, 0, 0, 0x80);
        }
    } else {
        t = D_0015F438 + st * 0x2AB;
        size = D_001DDF68[st];
        off = size * 2;
        ph = t % 0x800;
        func_00233980(0x47, 0);
        func_00233980(8, 0);
        FUN_00200468(D_001A00F0.tex0, size, size, 7, 7, (D_00151780.x << 4) - off, (D_00151780.y << 4) - off, ph, 0, 0x80);
        func_00233980(0x47, 0x360B);
        func_00233980(8, 5);
        FUN_00200468(D_001A00F0.tex1, size, size, 7, 7, (D_00151780.x << 4) - off, (D_00151780.y << 4) - off, 0, 0, 0x80);
        FUN_00200468(D_001A00F0.tex2, size, size, 7, 7, (D_00151780.x << 4) - off, (D_00151780.y << 4) - off, 0, 0, 0x80);
    }
    return 4;
}

extern __typeof__(FUN_00220850) func_00220850 __attribute__((alias("FUN_00220850")));
