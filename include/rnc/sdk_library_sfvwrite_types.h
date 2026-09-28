#ifndef RNC_SDK_LIBRARY_SFVWRITE_TYPES_H
#define RNC_SDK_LIBRARY_SFVWRITE_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u32 unk0;
    u8 pad_4[0x4];
    u32 unk8;
    u16 unkC;
    u8 pad_E[0x2];
    s32 unk10;
    s32 unk14;
    u8 pad_18[0x4];
    s32 unk1C;
    u8 pad_20[0x4];
    s32 unk24;
};

struct M2c_arg1 {
    struct M2c_var_20_30 * unk0;
    u8 pad_4[0x4];
    s32 unk8;
};

struct M2c_var_20_30 {
    s32 unk0;
    s32 unk4;
};

#endif /* RNC_SDK_LIBRARY_SFVWRITE_TYPES_H */
