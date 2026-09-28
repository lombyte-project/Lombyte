#ifndef RNC_AUDIO_SOUND_FUN_0022DA68_TYPES_H
#define RNC_AUDIO_SOUND_FUN_0022DA68_TYPES_H

#include "types.h"

struct M2c_arg2 {
    u8 pad_0[0x24];
    struct M2c_temp_2_13 * unk24;
};

struct M2c_temp_2_13 {
    u8 pad_0[0xD];
    s32 unkD;
    u8 pad_11[0x17];
    s32 unk28;
};

struct M2c_temp_3_39 {
    u8 pad_0[0x7E];
    s32 unk7E;
    u8 pad_82[0x6];
    s32 unk88;
};

#endif /* RNC_AUDIO_SOUND_FUN_0022DA68_TYPES_H */
