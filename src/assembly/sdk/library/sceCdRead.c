#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit sceCdRead; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/sceCdRead/sceCdRead.s", sceCdRead);
#else
#include "types.h"
struct M2c_D_001313C0 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 pad_F[0x1];
    s32 unk10;
    s32 unk14;
};

struct M2c_arg3 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
};

extern u8 D_00120788[];
extern s32 D_001312D0[];
extern volatile u32 D_001312E8[];
extern volatile u32 D_001312F0[];
extern volatile u32 D_001312F4[];
extern volatile u32 D_00131314[];
extern struct M2c_D_001313C0 D_001313C0;
extern u8 D_001323C0[];
extern u8 D_00132480[];
extern u8 D_00132490[];
extern u8 D_00152FC8[];
extern u8 D_00152FE0[];
extern s32 SignalSema();
extern s32 func_00120A28();
extern s32 sceCdNcmdDiskReady();
extern s32 scePrintf();
extern s32 sceSifCallRpc();
extern s32 sceSifWriteBackDCache();
s32 sceCdRead(s32 arg0, s32 arg1, s32 *arg2, struct M2c_arg3 *arg3) {
    s32 var_5_50;
    s32 temp_7_46;
    struct M2c_D_001313C0 *st = &D_001313C0;

    if (D_001312F4[0] & 1) {
        goto block_2;
    }
    if (sceCdNcmdDiskReady() == 6) {
        return 0;
    }
block_2:
    if (func_00120A28(4) == 0) {
        return 0;
    }
    st->unk0 = arg0;
    st->unk4 = arg1;
    st->unk8 = arg2;
    st->unkC = (u8) arg3->unk0;
    st->unkD = (u8) arg3->unk1;
    st->unkE = (u8) arg3->unk2;
    st->unk10 = D_001323C0;
    st->unk14 = D_00132480;
    temp_7_46 = arg3->unk2;
    if (temp_7_46 == 1) {
        goto block_7;
    }
    var_5_50 = arg1 << 0xB;
    if (temp_7_46 < 2) {
        goto block_9;
    }
    if (temp_7_46 == 2) {
        goto block_8;
    }
    goto block_9;
block_7:
    var_5_50 = arg1 * 0x918;
    goto block_9;
block_8:
    var_5_50 = arg1 * 0x924;
block_9:
    *(s32 *)D_00132480 = 0;
    if (D_001312F4[0] & 2) {
        goto block_11;
    }
    sceSifWriteBackDCache(arg2, var_5_50);
block_11:
    sceSifWriteBackDCache(D_001323C0, 0x90);
    sceSifWriteBackDCache(st, 0x18);
    sceSifWriteBackDCache(D_00132480, 4);
    if (D_001312D0[0] <= 0) {
        goto block_13;
    }
    scePrintf(D_00152FC8);
block_13:
    D_00131314[0] = 1;
    D_001312F0[0] = 1;
    if (sceSifCallRpc(D_00132490, 1, 1, st, 0x18, 0, 0, D_00120788, D_001323C0) < 0) {
        goto block_15;
    }
    goto block_18;
block_15:
    D_00131314[0] = 0;
    D_001312F0[0] = 0;
    SignalSema(D_001312E8[0]);
    return 0;
block_18:
    if (D_001312D0[0] <= 0) {
        return 1;
    }
    scePrintf(D_00152FE0);
    return 1;
}
#endif /* NON_MATCHING */
