#ifndef RNC_FUN_001EBF10_TYPES_H
#define RNC_FUN_001EBF10_TYPES_H

#include "types.h"

struct M2c_D_00186F40 {
    u8 pad_0[0x140];
    s64 unk140;
    u8 pad_148[0x38];
    s32 unk180;
    struct M2c_D_00186F40_unk184 * unk184;
    u8 pad_188[0xE8];
    s32 unk270;
    s32 unk273;
    u8 pad_277[0x11];
    s32 unk288;
    u8 pad_28C[0x8];
    s32 unk294;
    u8 pad_298[0x5C];
    s32 unk2F4;
    u8 pad_2F8[0xA0];
    s32 unk398;
};

struct M2c_D_00186F40_unk184 {
    u8 pad_0[0x70];
    s32 unk70;
};

struct M2c_arg0 {
    s64 unk0;
    u8 pad_8[0x8];
    s64 unk10;
    u8 pad_18[0x8];
    s64 unk20;
    u8 pad_28[0x8];
    s32 unk30;
    u8 pad_34[0x30];
    s32 unk64;
    u8 pad_68[0x8];
    s32 unk70;
    u8 pad_74[0x4];
    s32 unk78;
    u8 pad_7C[0x1];
    s32 unk7D;
    u8 pad_81[0x3];
    s32 unk84;
    u8 pad_88[0x6];
    s32 unk8E;
};

struct M2c_temp_20_19 {
    s32 unk0;
    u8 pad_4[0xC];
    s32 unk10;
    u8 pad_14[0xC];
    s32 unk20;
    u8 pad_24[0xC];
    s32 unk30;
    u8 pad_34[0x49];
    u8 unk7D;
    s16 unk7E;
    u8 pad_80[0xE];
    s16 unk8E;
    u8 pad_90[0x2];
};

struct M2c_temp_2_201 {
    u8 pad_0[0x4];
    s32 unk4;
    s32 unk8;
};

struct M2c_temp_2_22 {
    u8 pad_0[0x1D];
    s32 unk1D;
};

struct M2c_var_19_31 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

#endif /* RNC_FUN_001EBF10_TYPES_H */
