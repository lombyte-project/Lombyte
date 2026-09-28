#ifndef RNC_AUDIO_VOICES_PAUSE_ALL_SOUNDS_TYPES_H
#define RNC_AUDIO_VOICES_PAUSE_ALL_SOUNDS_TYPES_H

#include "types.h"

struct M2c_D_001CE5B8 {
    u8 pad_0[0x38];
    s32 unk38;
};

struct M2c_D_001CE748 {
    u8 pad_0[0x3C];
    s32 unk3C;
};

struct M2c_D_001D5BF0 {
    s32 unk0;
    u8 pad_4[0x8];
    s32 unkC;
    s32 unk10;
    u8 pad_14[0xC4];
    s32 unkD8;
    s32 unkDC;
    u8 pad_E0[0x18];
    s32 unkF8;
    u8 pad_FC[0x14];
    s32 unk110;
    u8 pad_114[0x20];
    s32 unk134;
    s32 unk138;
    s32 unk13C;
    s32 unk140;
};

#endif /* RNC_AUDIO_VOICES_PAUSE_ALL_SOUNDS_TYPES_H */
