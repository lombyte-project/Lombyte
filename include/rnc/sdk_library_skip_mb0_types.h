#ifndef RNC_SDK_LIBRARY_SKIP_MB0_TYPES_H
#define RNC_SDK_LIBRARY_SKIP_MB0_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x150];
    s32 unk150;
    u8 pad_154[0x20];
    s32 unk174;
    u8 pad_178[0x38];
    s32 unk1B0;
    u8 pad_1B4[0x65C];
    s32 unk810;
};

struct M2c_arg1 {
    s32 unk0;
    s32 unk4;
    u8 pad_8[0x8];
    s32 unk10;
    s32 unk14;
};

struct M2c_arg3 {
    s32 unk0;
    s32 unk4;
};

#endif /* RNC_SDK_LIBRARY_SKIP_MB0_TYPES_H */
