#ifndef RNC_SDK_LIBRARY_MOTION_VECTORS_TYPES_H
#define RNC_SDK_LIBRARY_MOTION_VECTORS_TYPES_H

#include "types.h"

struct M2c_temp_16_46 {
    s32 unk0;
    u8 pad_4[0x4];
    s32 unk8;
};

struct M2c_temp_16_84 {
    s32 unk0;
    s32 unk4;
    u8 pad_8[0x8];
    s32 unk10;
    s32 unk14;
};

struct M2c_temp_3_33 {
    s32 unk0;
    u8 pad_4[0x4];
    s32 unk8;
};

#endif /* RNC_SDK_LIBRARY_MOTION_VECTORS_TYPES_H */
