#ifndef RNC_SDK_RPC_REQUEST_BIND_TYPES_H
#define RNC_SDK_RPC_REQUEST_BIND_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x4];
    s32 unk1C;
    s32 unk20;
};

struct M2c_temp_2_10 {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x4];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
};

struct M2c_temp_2_21 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x8];
    s32 unk14;
};

#endif /* RNC_SDK_RPC_REQUEST_BIND_TYPES_H */
