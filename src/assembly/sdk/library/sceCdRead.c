#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit sceCdRead; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/sceCdRead/sceCdRead.s", sceCdRead);
#else
#include "types.h"
struct CdDriveState {
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

struct sceCdRMode {
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
extern struct CdDriveState D_001313C0;
extern u8 D_001323C0[];
extern u8 D_00132480[];
extern u8 D_00132490[];
extern u8 D_00152FC8[];
extern u8 D_00152FE0[];
extern s32 SignalSema(u32 semaphore);
extern s32 cd_check_ncmd() __asm__("func_00120A28");
extern s32 sceCdNcmdDiskReady();
extern s32 scePrintf();
extern s32 sceSifCallRpc();
extern s32 sceSifWriteBackDCache();
s32 sceCdRead(u32 dwSector, u32 dwSectorCount, s32 *pDestination, struct sceCdRMode *pMode) {
    s32 byteCount;
    s32 dataPattern;
    struct CdDriveState *st = &D_001313C0;

    if (D_001312F4[0] & 1) {
        goto block_2;
    }
    if (sceCdNcmdDiskReady() == 6) {
        return 0;
    }
block_2:
    if (cd_check_ncmd(4) == 0) {
        return 0;
    }
    st->unk0 = dwSector;
    st->unk4 = dwSectorCount;
    st->unk8 = pDestination;
    st->unkC = (u8)pMode->unk0;
    st->unkD = (u8)pMode->unk1;
    st->unkE = (u8)pMode->unk2;
    st->unk10 = D_001323C0;
    st->unk14 = D_00132480;
    dataPattern = pMode->unk2;
    if (dataPattern == 1) {
        goto block_7;
    }
    byteCount = dwSectorCount << 0xB;
    if (dataPattern < 2) {
        goto block_9;
    }
    if (dataPattern == 2) {
        goto block_8;
    }
    goto block_9;
block_7:
    byteCount = dwSectorCount * (dataPattern = 0x918);
    goto block_9;
block_8:
    byteCount = dwSectorCount * 0x924;
block_9:
    *(s32 *)D_00132480 = 0;
    if (D_001312F4[0] & 2) {
        goto block_11;
    }
    sceSifWriteBackDCache(pDestination, byteCount);
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
