#ifndef RNC_GAMEPLAY_STATE_COMPUTE_INTERPOLATED_RECORD_VALUE_TYPES_H
#define RNC_GAMEPLAY_STATE_COMPUTE_INTERPOLATED_RECORD_VALUE_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x50];
    u8 unk50;
    u8 unk51;
    u8 unk52;
    u8 unk53;
    s32 unk54;
    u8 pad_58[0x10];
    struct M2c_var_4_14 * unk68;
    struct M2c_temp_16_50 * unk6C;
};

struct M2c_temp_16_50 {
    u8 pad_0[0x4];
    s32 unk4;
};

struct M2c_var_4_14 {
    u8 pad_0[0x4];
    s32 unk4;
};

#endif /* RNC_GAMEPLAY_STATE_COMPUTE_INTERPOLATED_RECORD_VALUE_TYPES_H */
