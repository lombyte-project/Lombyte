#ifndef RNC_GAMEPLAY_CAMERA_UPDATE_CAMERA_UNDERWATER_FLAG_TYPES_H
#define RNC_GAMEPLAY_CAMERA_UPDATE_CAMERA_UNDERWATER_FLAG_TYPES_H

#include "types.h"

struct M2c_D_00186F40 {
    u8 pad_0[0x140];
    s32 unk140;
    u8 pad_144[0x4];
    s32 unk148;
    u8 pad_14C[0x34];
    struct M2c_D_00186F40_unk180 * unk180;
    u8 pad_184[0x210];
    s32 unk394;
};

struct M2c_D_00186F40_unk180 {
    u8 pad_0[0x86];
    s32 unk86;
};

#endif /* RNC_GAMEPLAY_CAMERA_UPDATE_CAMERA_UNDERWATER_FLAG_TYPES_H */
