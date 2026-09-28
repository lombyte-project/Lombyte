#include "types.h"

extern u8 D_0013CDD0[];
extern s32 D_0015F438;
extern u8 *D_00160F00;
extern void func_001FB2D0(void);
extern void func_001FB368(void);
extern void func_001F5210(s32, s32, s32, s32);
extern void func_001FB3D0(void);
extern void func_002335D0(void);
extern void func_00233630(void);
extern void func_002336A0(void);
extern void func_002337B0(s32);
extern void func_00233980(s32, u64);
extern s32 sceGsSyncV(s32);

#define VP (*(u8 * volatile *)&D_00160F00)

void fade_to_black(s32 n) __asm__("FUN_001f4a58");

void fade_to_black(s32 n) {
    s32 i;

    func_002337B0(1);
    sceGsSyncV(0);
    D_0015F438 += 1;
    func_002335D0();
    for (i = n - 1; i >= 0; i--) {
        func_001FB2D0();
        func_001FB368();
        func_001F5210(0, 0, 0, 0x80);
        func_001FB3D0();
        func_00233980(1, (u64)(0x80 - (i * 0x80) / (i + 1)) << 24);
        *(u32 *)(VP + 0) = 0x30000014;
        *(u32 *)(VP + 4) = (u32)D_0013CDD0;
        *(u32 *)(VP + 8) = 0;
        *(u32 *)(VP + 12) = 0x50000014;
        D_00160F00 = VP + 0x10;
        func_002337B0(1);
        sceGsSyncV(0);
        D_0015F438 += 1;
        func_002336A0();
        func_00233630();
    }
    func_002337B0(1);
    sceGsSyncV(0);
    D_0015F438 += 1;
    func_002335D0();
    func_001FB2D0();
    func_001FB368();
}

extern __typeof__(VP) func_001F4A58 __attribute__((alias("FUN_001f4a58")));
