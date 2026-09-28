#ifndef RNC_GAMEPLAY_VENDOR_FUN_0023A318_TYPES_H
#define RNC_GAMEPLAY_VENDOR_FUN_0023A318_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x78];
    struct M2c_temp_16_12 * unk78;
};

struct M2c_temp_16_12 {
    u8 pad_0[0x40];
    s32 unk40;
    s32 unk44;
};

#endif /* RNC_GAMEPLAY_VENDOR_FUN_0023A318_TYPES_H */
