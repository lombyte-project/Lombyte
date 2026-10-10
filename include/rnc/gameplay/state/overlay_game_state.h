#ifndef LOMBYTE_RNC_GAMEPLAY_STATE_OVERLAY_GAME_STATE_H
#define LOMBYTE_RNC_GAMEPLAY_STATE_OVERLAY_GAME_STATE_H

#include "types.h"
#include "rnc/math/vector.h"

struct Mth;
struct NO;

struct Own {
    s32 ids[14];
    struct Own *unk38;
    s32 unk3C;
    struct Mth *unk40;
    struct NO *objs[14];
    u8 pad7C[4];
    struct Mth *unk80;
};

/* Shared Level 00 state; previously separate allocation, HUD and voice views. */
struct GameState {
    s32 state;
    struct Own *owner;
    struct Own *unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x28];
    Vec4 v40;
    Vec4 v50;
    u8 pad60[0x70];
    struct Own *unkD0;
    u8 padD4[0x10];
    void *unkE4;
    f32 unkE8;
    void *unkEC;
    u8 padF0[0xC];
    s32 first_base;                /* 0xFC: render archive allocation */
    s32 second_base;               /* 0x100 */
    s32 unk104;
    s32 unk108;
    s32 unk10C;
    s32 progress;
    u8 pad114[0x124 - 0x114];
    s32 unk124;
    u8 pad128[0x130 - 0x128];
    s32 unk130;
    u8 pad134[0x140 - 0x134];
    s32 unk140;
    s32 unk144;
    s32 stash_handles[20];         /* 0x148: FUN_L00_00276078 */
};

extern struct GameState D_L00_001B9CF0 __attribute__((section(".data")));

#endif
