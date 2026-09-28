#ifndef RNC_RUNTIME_STATE_READ_STATE_FIELD_TYPES_H
#define RNC_RUNTIME_STATE_READ_STATE_FIELD_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x34];
    u16 unk34;
    u8 pad_36[0x42];
    struct M2c_arg0_unk78 * unk78;
};

struct M2c_arg0_unk78 {
    u8 pad_0[0x10];
    s32 unk10;
};

#endif /* RNC_RUNTIME_STATE_READ_STATE_FIELD_TYPES_H */
