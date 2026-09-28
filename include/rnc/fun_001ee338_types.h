#ifndef RNC_FUN_001EE338_TYPES_H
#define RNC_FUN_001EE338_TYPES_H

#include "types.h"

struct M2c_D_0013E500 {
    u8 pad_0[0x8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
};

struct M2c_D_00189300 {
    u8 pad_0[0xC0];
    s32 unkC0;
};

struct M2c_temp_2_45 {
    u8 pad_0[0x20];
    s32 unk20;
};

struct M2c_var_16_43 {
    u8 pad_0[0x20];
    struct M2c_temp_2_45 * unk20;
    s16 unk24;
    u8 pad_26[0x2];
};

#endif /* RNC_FUN_001EE338_TYPES_H */
