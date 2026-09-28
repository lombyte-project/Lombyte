#ifndef RNC_SDK_LIBRARY__PEEPBIT_TYPES_H
#define RNC_SDK_LIBRARY__PEEPBIT_TYPES_H

#include "types.h"

struct M2c_D_00132E70 {
    u8 pad_0[0x10];
    s32 unk10;
};

struct M2c_arg0 {
    u8 pad_0[0x818];
    s32 unk818;
    u8 pad_81C[0x1C];
    u32 unk838;
    s32 unk83C;
    u8 pad_840[0x18];
    s32 unk858;
};

#endif /* RNC_SDK_LIBRARY__PEEPBIT_TYPES_H */
