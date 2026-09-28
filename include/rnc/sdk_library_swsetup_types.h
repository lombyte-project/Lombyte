#ifndef RNC_SDK_LIBRARY_SWSETUP_TYPES_H
#define RNC_SDK_LIBRARY_SWSETUP_TYPES_H

#include "types.h"

struct M2c_arg0 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u16 unkC;
    u8 pad_E[0x2];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad_1C[0x14];
    s32 unk30;
    u8 pad_34[0x20];
    struct M2c_var_3_7 * unk54;
};

struct M2c_var_3_7 {
    u8 pad_0[0x38];
    s32 unk38;
};

#endif /* RNC_SDK_LIBRARY_SWSETUP_TYPES_H */
