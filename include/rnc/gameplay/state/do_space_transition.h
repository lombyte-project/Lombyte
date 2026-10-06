#ifndef LOMBYTE_RNC_GAMEPLAY_STATE_DO_SPACE_TRANSITION_H
#define LOMBYTE_RNC_GAMEPLAY_STATE_DO_SPACE_TRANSITION_H

#include "types.h"

struct SaveSlotTable {
    u8 pad_0[0xD4];
    s32 unkD4;
    u8 pad_D8[0x4];
    s32 unkDC;
};

struct Globals_0013DD40 {
    u8 pad_0[0x8];
    u8 unk8;
    u8 pad_9[0x5];
    u8 unkE;
    u8 unkF;
    u8 pad_10[0x3];
};

struct Globals_0013DD58 {
    u8 unk0;
    u8 unk1;
};

struct Globals_0013E030 {
    u8 pad_0[0x26];
    s16 unk26;
    u8 pad_28[0x2];
};

#include "rnc/audio/music/music_stream_state.h"

struct Globals_0015F634 {
    u8 pad_0[0x1C];
    s32 unk1C;
};

struct Globals_0018CD00 {
    u8 pad_0[0x218];
    s32 unk218;
    f32 unk21C;
    u8 pad_220[0x8];
    f32 unk228;
    f32 unk22C;
    s32 unk230;
    s32 unk234;
    s32 unk238;
};

struct Globals_00194100 {
    u8 pad_0[0x10];
    s32 unk10;
};

#endif /* LOMBYTE_RNC_GAMEPLAY_STATE_DO_SPACE_TRANSITION_H */
