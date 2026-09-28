#ifndef RNC_FUN_002116B8_TYPES_H
#define RNC_FUN_002116B8_TYPES_H

#include "types.h"

struct M2c_var_1_6 {
    s32 unk0;
    struct M2c_var_5_10 * unk4;
    struct M2c_var_6_11 * unk8;
    s32 unkC;
};

struct M2c_var_5_10 {
    u8 pad_0[0xF];
    s32 unkF;
};

struct M2c_var_6_11 {
    u8 pad_0[0x4];
    struct M2c_var_8_14 * unk4;
};

struct M2c_var_8_14 {
    s32 unk0;
    s32 unk4;
};

#endif /* RNC_FUN_002116B8_TYPES_H */
