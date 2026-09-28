#ifndef RNC_SDK_LIBRARY_FFLUSH_TYPES_H
#define RNC_SDK_LIBRARY_FFLUSH_TYPES_H

#include "types.h"

struct M2c_arg0 {
    s32 unk0;
    u8 pad_4[0x4];
    s32 unk8;
    s16 unkC;
    u8 pad_E[0x2];
    s32 unk10;
    s32 unk14;
    u8 pad_18[0x4];
    s32 unk1C;
    u8 pad_20[0x4];
    s32 unk24;
    u8 pad_28[0x2C];
    struct M2c_var_3_18 * unk54;
};

struct M2c_var_3_18 {
    u8 pad_0[0x38];
    s32 unk38;
};

#endif /* RNC_SDK_LIBRARY_FFLUSH_TYPES_H */
