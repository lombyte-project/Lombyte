#ifndef RNC_RUNTIME_RPC_GET_AVAILABLE_RPC_PACKET_TYPES_H
#define RNC_RUNTIME_RPC_GET_AVAILABLE_RPC_PACKET_TYPES_H

#include "types.h"

struct M2c_arg0 {
    s32 unk0;
    struct M2c_var_16_14 * unk4;
    s32 unk8;
};

struct M2c_var_16_14 {
    u8 pad_0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
};

#endif /* RNC_RUNTIME_RPC_GET_AVAILABLE_RPC_PACKET_TYPES_H */
