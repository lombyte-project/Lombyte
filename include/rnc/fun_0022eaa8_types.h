#ifndef RNC_FUN_0022EAA8_TYPES_H
#define RNC_FUN_0022EAA8_TYPES_H

#include "types.h"

struct M2c_D_0013DD40 {
    u8 pad_0[0x3];
    s32 unk3;
};

struct M2c_D_0013E030 {
    u8 pad_0[0x50];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
};

struct M2c_D_0018CB20 {
    u8 pad_0[0x34];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s16 unk40;
    u8 pad_42[0x2];
    s16 unk44;
    u8 pad_46[0x12];
    s32 unk58;
    s32 unk5C;
};

struct M2c_D_0018CD00 {
    u8 pad_0[0xB0];
    f32 unkB0;
};

struct M2c_D_001940C0 {
    u8 pad_0[0x4];
    s32 unk4;
    s32 unk8;
    u8 pad_C[0x8];
    struct M2c_D_001940C0_unk14 * unk14;
};

struct M2c_D_001940C0_unk14 {
    u8 pad_0[0x4];
    s32 unk4;
};

struct M2c_temp_21_323 {
    u8 pad_0[0x24];
    struct M2c_temp_21_323_unk24 * unk24;
    u8 pad_28[0x4];
    f32 unk2C;
    u8 pad_30[0x2];
    u16 unk32;
    u16 unk34;
    u8 pad_36[0x1A];
    u8 unk50;
    u8 unk51;
    s8 unk52;
    s8 unk53;
    f32 unk54;
    u8 pad_58[0x10];
    s32 unk68;
    s32 unk6C;
    u8 pad_70[0x1];
    s8 unk71;
    s32 unk72;
    u8 pad_76[0x2];
    s32 unk78;
};

struct M2c_temp_21_323_unk24 {
    u8 pad_0[0x24];
    s32 unk24;
};

struct M2c_temp_3_88 {
    u8 pad_0[0x24];
    struct M2c_temp_3_95 * unk24;
};

struct M2c_temp_3_95 {
    u8 pad_0[0xC];
    u8 unkC;
    u8 pad_D[0x3];
};

struct M2c_temp_5_324 {
    u8 pad_0[0xC];
    u8 unkC;
    u8 pad_D[0x3];
};

struct M2c_temp_5_333 {
    u8 pad_0[0x48];
    s32 unk48;
};

struct M2c_temp_5_91 {
    u8 pad_0[0xC];
    u8 unkC;
};

struct M2c_temp_7_398 {
    u8 pad_0[0xC];
    u8 unkC;
    u8 pad_D[0x3];
};

struct M2c_var_5_183 {
    u8 pad_0[0x4];
    s32 unk4;
};

#endif /* RNC_FUN_0022EAA8_TYPES_H */
