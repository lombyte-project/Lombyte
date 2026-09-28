#ifndef RNC_SDK_LIBRARY_SMAKEBUF_TYPES_H
#define RNC_SDK_LIBRARY_SMAKEBUF_TYPES_H

#include "types.h"

struct M2c_arg0 {
    s32 unk0;
    u8 pad_4[0x8];
    u16 unkC;
    u16 unkE;
    s32 unk10;
    s32 unk14;
    u8 pad_18[0x10];
    s32 unk28;
    u8 pad_2C[0x20];
    s32 unk4C;
    u8 pad_50[0x4];
    struct M2c_arg0_unk54 * unk54;
};

struct M2c_arg0_unk54 {
    u8 pad_0[0x3C];
    s32 unk3C;
};

#endif /* RNC_SDK_LIBRARY_SMAKEBUF_TYPES_H */
