#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/video/display/set_pal_mode/FUN_001f34e8.s", FUN_001f34e8);
#else
#include "types.h"

typedef struct { long q[12]; } sceGsLoadImage __attribute__((aligned(16)));

typedef struct {
    s32 w;
    s32 h;
    s32 hw;
    s32 hh;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} Screen;

typedef struct {
    u8 pad0[0x150];
    s16 w;
    s16 h;
    u8 pad154[4];
    s16 pw;
    s16 ph;
} View;

typedef struct {
    u64 pad0[2];
    u64 frame1;
    u64 pad18;
    u64 frame2;
    u64 pad28;
    u64 zbuf1;
    u64 pad38;
    u64 zbuf2;
    u64 pad48;
    u64 xyoffset1;
    u64 pad58;
    u64 xyoffset2;
    u64 pad68;
    u64 scissor1;
    u64 pad78;
    u64 scissor2;
} DrawEnv;

extern Screen D_0013E500;
extern View D_00151780;
extern DrawEnv D_0013CF10;
extern u64 D_0013D100;
extern u64 D_0013D170;
extern s32 D_0015ED80;
extern s32 D_0015EE74;
extern s32 D_0015EE78;
extern s32 D_0015EE80;
extern s32 D_0015EE84;
extern s32 D_0015EE88;
extern s32 D_0015EE8C;
extern u8 D_001941C0[];
extern void FillTransferWords(u8 *, s32, s32);
extern void FlushCache(s32);
extern void func_00120558(s32, s32);
extern void func_001FA978(s32, s32, s32, s32, s32, s32);
extern void func_001FB2A8(void);
extern void func_001FB2D0(void);
extern void func_001FB368(void);
extern s32 sceGsExecLoadImage(sceGsLoadImage *, u8 *);
extern s32 sceGsSetDefLoadImage(sceGsLoadImage *, s16, s16, s16, s16, s16, s16, s16);

void set_pal_mode(void) __asm__("FUN_001f34e8");

void set_pal_mode(void)
{
    sceGsLoadImage li;
    s32 n;
    s32 i;
    u64 zbuf;
    u64 frame;
    u64 scissor;

    FlushCache(0);
    if (D_0015ED80 != 0) {
        D_0015EE84 = 0x100000;
        D_0015EE88 = 0x1E0000;
        D_0015EE80 = 0;
        D_0015EE8C = 0x2C0000;
        func_001FA978(0x200, 0x1C0, 0x200, 0x200, 4, 0);
    } else {
        D_0015EE84 = 0xE0000;
        D_0015EE88 = 0x1B0000;
        D_0015EE80 = 0;
        D_0015EE8C = 0x280000;
        func_001FA978(0x200, 0x1A0, 0x200, 0x1C0, 0, 0);
    }
    D_0013E500.w = D_00151780.w;
    D_0013E500.h = D_00151780.h;
    D_0013E500.hw = D_00151780.w >> 1;
    D_0013E500.hh = D_00151780.h >> 1;
    D_0013E500.x0 = (0x800 - D_0013E500.hw) << 4;
    D_0013E500.y0 = (0x800 - D_0013E500.hh) << 4;
    D_0013E500.x1 = (D_0013E500.hw + 0x800) << 4;
    D_0013E500.y1 = (D_0013E500.hh + 0x800) << 4;
    FlushCache(0);
    func_00120558(0, 0);
    zbuf = (D_0015EE88 >> 13) | 0x1000000;
    frame = (D_0015EE84 >> 13) | ((u64)(D_0013E500.w >> 6) << 16);
    scissor = ((u64)(D_0013E500.w - 1) << 16) | ((u64)(D_0013E500.h - 1) << 48);
    D_0013CF10.scissor2 = scissor;
    D_0013D100 = zbuf;
    D_0015EE74 = D_0015EE8C;
    D_0013D170 = zbuf | ((u64)0x8000 << 17);
    D_0015EE78 = D_0015EE8C;
    D_0013CF10.frame2 = frame;
    D_0013CF10.xyoffset1 = D_0013E500.x0 | ((u64)D_0013E500.y0 << 32);
    D_0013CF10.xyoffset2 = D_0013E500.x0 | ((u64)D_0013E500.y0 << 32);
    D_0013CF10.zbuf1 = zbuf;
    D_0013CF10.zbuf2 = zbuf;
    D_0013CF10.frame1 = frame;
    D_0013CF10.scissor1 = scissor;
    FlushCache(0);
    func_001FB2D0();
    func_001FB368();
    FlushCache(0);
    func_00120558(0, 0);
    func_001FB2A8();
    FillTransferWords(D_001941C0, 0, 0x1000);
    n = (D_00151780.pw * D_00151780.ph) >> 10;
    for (i = 0; i < n; i++) {
        sceGsSetDefLoadImage(&li, i << 4, 1, 0, 0, 0, 32, 32);
        FlushCache(0);
        sceGsExecLoadImage(&li, D_001941C0);
        func_00120558(0, 0);
    }
}
#endif /* NON_MATCHING */
