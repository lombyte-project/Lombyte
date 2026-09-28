#ifndef RNC_SDK_RPC_REQUEST_RDATA_TYPES_H
#define RNC_SDK_RPC_REQUEST_RDATA_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x4];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
};

struct M2c_temp_2_7 {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x4];
    s32 unk1C;
    s32 unk20;
};

#endif /* RNC_SDK_RPC_REQUEST_RDATA_TYPES_H */
