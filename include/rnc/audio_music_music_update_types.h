#ifndef RNC_AUDIO_MUSIC_MUSIC_UPDATE_TYPES_H
#define RNC_AUDIO_MUSIC_MUSIC_UPDATE_TYPES_H

#include "types.h"

#include "rnc/music_stream_state.h"

struct M2c_temp_17_273 {
    u8 pad_0[0x8];
    s16 unk8;
    u16 unkA;
    s32 unkC;
    s32 unk10;
    s32 unk14;
};

struct M2c_temp_17_66 {
    u8 pad_0[0x20];
    s16 unk20;
    u8 unk22;
    s32 unk23;
    u8 pad_27[0x5];
    s32 unk2C;
    u8 pad_30[0x8];
    s16 unk38;
    s16 unk3A;
    s16 unk3C;
    u8 pad_3E[0x2];
};

#endif /* RNC_AUDIO_MUSIC_MUSIC_UPDATE_TYPES_H */
