#include "types.h"
struct M2c_D_0013D290 {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0xA8];
    s32 unkC0;
    u8 pad_C4[0x18];
    s32 unkDC;
    s32 unkE0;
    u8 pad_E4[0x8];
    s32 unkEC;
    u8 pad_F0[0x4];
    s32 unkF4;
};

extern struct M2c_D_0013D290 D_0013D290;
extern u8 D_0015EE98[];
extern s32 func_00209370();
extern s32 func_0020ABB0();
extern s32 sceCdReadClock();
extern s32 sceScfGetLocalTimefromRTC();
void FUN_00226a70(s32 arg0, s32 arg1) {
    func_00209370();
    {
        u8 *p1 = D_0015EE98;
        __asm__ volatile ("" : : "r" (p1));
        sceCdReadClock(p1);
    }
    {
        u8 *p1 = D_0015EE98;
        __asm__ volatile ("" : : "r" (p1));
        sceScfGetLocalTimefromRTC(p1);
    }
    func_0020ABB0(arg0);
    D_0013D290.unkC0 = 0;
    D_0013D290.unk14 = arg1;
    *(s32 *)((u8 *)&D_0013D290 + (arg1 * 0x1C) + 0x20) = 0;
    D_0013D290.unkF4 = 1;
    D_0013D290.unkEC = arg0;
    if (D_0013D290.unkDC < 0) {
        D_0013D290.unkE0 = 0;
        D_0013D290.unkDC = 0x13;
    }
}
