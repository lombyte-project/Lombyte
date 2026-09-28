#ifndef RNC_RENDERING_TEXTURE_PATCH_TIE_TEXTURE_FIELDS_TYPES_H
#define RNC_RENDERING_TEXTURE_PATCH_TIE_TEXTURE_FIELDS_TYPES_H

#include "types.h"

struct M2c_temp_5_27 {
    u16 unk0;
    s32 unk2;
};

struct M2c_temp_8_17 {
    u8 pad_0[0x23];
    u8 unk23;
    u8 pad_24[0x8];
    struct M2c_var_4_20 * unk2C;
};

struct M2c_var_4_20 {
    s32 unk0;
    u8 pad_4[0x1C];
    s32 unk20;
    u8 pad_24[0xF];
    s32 unk33;
};

#endif /* RNC_RENDERING_TEXTURE_PATCH_TIE_TEXTURE_FIELDS_TYPES_H */
