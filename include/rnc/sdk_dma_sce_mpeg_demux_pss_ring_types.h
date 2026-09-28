#ifndef RNC_SDK_DMA_SCE_MPEG_DEMUX_PSS_RING_TYPES_H
#define RNC_SDK_DMA_SCE_MPEG_DEMUX_PSS_RING_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x40];
    struct M2c_temp_20_22 * unk40;
};

struct M2c_sp {
    u8 pad_0[0x18];
    s32 unk18;
};

struct M2c_sp30 {
    u8 pad_0[0x18];
    s32 unk18;
    u8 pad_1C[0xC];
    s32 unk28;
    u8 pad_2C[0x4];
    s32 unk30;
    u8 pad_34[0x4];
    s32 unk38;
    s32 unk3C;
    s32 unk40;
};

struct M2c_temp_16_115 {
    s32 unk0;
    u8 pad_4[0x4];
    s32 unk8;
    u8 pad_C[0x4];
    s32 unk10;
    s32 unk14;
};

struct M2c_temp_20_22 {
    u8 pad_0[0x44];
    s32 unk44;
    s32 unk48;
};

struct M2c_temp_2_40 {
    s32 unk0;
    u8 pad_4[0xC];
    s32 unk10;
    s32 unk14;
};

#endif /* RNC_SDK_DMA_SCE_MPEG_DEMUX_PSS_RING_TYPES_H */
