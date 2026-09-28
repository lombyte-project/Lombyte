#ifndef RNC_SDK_DMA_SCE_MPEG_ADD_STR_CALLBACK_TYPES_H
#define RNC_SDK_DMA_SCE_MPEG_ADD_STR_CALLBACK_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x40];
    struct M2c_temp_18_16 * unk40;
};

struct M2c_temp_16_19 {
    s32 unk0;
    u8 pad_4[0xC];
    s32 unk10;
};

struct M2c_temp_18_16 {
    u8 pad_0[0x44];
    struct M2c_temp_16_19 * unk44;
    s32 unk48;
};

struct M2c_temp_2_34 {
    s32 unk0;
    u8 pad_4[0xC];
    s32 unk10;
};

struct M2c_temp_3_50 {
    s32 unk0;
    u8 pad_4[0x4];
    s64 unk8;
    s32 unk10;
    s32 unk14;
};

#endif /* RNC_SDK_DMA_SCE_MPEG_ADD_STR_CALLBACK_TYPES_H */
