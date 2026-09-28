#ifndef RNC_RENDERING_STATE_RESET_GS_REGISTERS_TYPES_H
#define RNC_RENDERING_STATE_RESET_GS_REGISTERS_TYPES_H

#include "types.h"

struct M2c_D_00160F00 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

struct M2c_D_0018CD00 {
    u8 pad_0[0x230];
    s32 unk230;
    s32 unk234;
    s32 unk238;
};

struct M2c_temp_3_33 {
    u8 pad_0[0x10];
    s32 unk10;
};

#endif /* RNC_RENDERING_STATE_RESET_GS_REGISTERS_TYPES_H */
