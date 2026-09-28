#ifndef RNC_GAMEPLAY_GADGETS_LOAD_HAND_GADGET_TYPES_H
#define RNC_GAMEPLAY_GADGETS_LOAD_HAND_GADGET_TYPES_H

#include "types.h"

struct M2c_D_0013D4C0 {
    u8 pad_0[0x21];
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 pad_24[0x3];
};

struct M2c_D_0013F350 {
    u8 pad_0[0x10B8];
    s32 unk10B8;
    u8 pad_10BC[0xF3A];
    u8 unk1FF6;
    u8 unk1FF7;
    u8 pad_1FF8[0x3];
};

struct M2c_D_001D5BF0 {
    u8 pad_0[0x1C];
    s32 unk1C;
    u8 pad_20[0x10];
    s32 unk30;
    s16 unk34;
    u8 pad_36[0x2];
    s32 unk38;
    s32 unk3C;
    u8 pad_40[0x8C];
    s32 unkCC;
    u8 pad_D0[0x48];
    s32 unk118;
    s32 unk11C;
    s32 unk120;
    u8 pad_124[0x1C];
    s32 unk140;
    s32 unk144;
};

struct M2c_D_001D5DD0 {
    u8 pad_0[0x1];
    u8 unk1;
    u8 pad_2[0x1E];
    s32 unk20;
    s32 unk24;
    s32 unk28;
};

struct M2c_D_001D5E10 {
    u8 pad_0[0x1];
    u8 unk1;
    u8 pad_2[0x1E];
    f32 unk20;
    f32 unk24;
    f32 unk28;
};

struct M2c_D_001D5E50 {
    u8 pad_0[0x1];
    u8 unk1;
    u8 pad_2[0x1E];
    f32 unk20;
    f32 unk24;
    f32 unk28;
};

struct M2c_arg0 {
    u8 pad_0[0x44];
    s32 unk44;
    u8 pad_48[0x4];
    struct M2c_temp_17_248 * unk4C;
    struct M2c_temp_17_118 * unk50;
    struct M2c_temp_17_17 * unk54;
    struct M2c_temp_17_155 * unk58;
    s32 unk5C;
    u8 pad_60[0xC];
    struct M2c_temp_17_298 * unk6C;
    struct M2c_temp_17_331 * unk70;
    struct M2c_temp_17_361 * unk74;
};

struct M2c_temp_17_118 {
    u8 pad_0[0xA6];
    s16 unkA6;
    u8 pad_A8[0x2];
};

struct M2c_temp_17_155 {
    u8 pad_0[0xA6];
    s16 unkA6;
    u8 pad_A8[0x2];
};

struct M2c_temp_17_17 {
    u8 pad_0[0xA6];
    s16 unkA6;
    u8 pad_A8[0x2];
};

struct M2c_temp_17_248 {
    u8 pad_0[0xA6];
    s16 unkA6;
    u8 pad_A8[0x2];
};

struct M2c_temp_17_298 {
    u8 pad_0[0xA6];
    s16 unkA6;
    u8 pad_A8[0x2];
};

struct M2c_temp_17_331 {
    u8 pad_0[0xA6];
    s16 unkA6;
    u8 pad_A8[0x2];
};

struct M2c_temp_17_361 {
    u8 pad_0[0xA6];
    s16 unkA6;
    u8 pad_A8[0x2];
};

struct M2c_temp_17_405 {
    u8 pad_0[0xA6];
    s16 unkA6;
    u8 pad_A8[0x2];
};

struct M2c_temp_2_475 {
    u8 pad_0[0x4];
    s32 unk4;
    s32 unk8;
    u8 pad_C[0x4];
    s32 unk10;
    u8 pad_14[0x4];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
};

struct M2c_temp_2_83 {
    u8 pad_0[0x34];
    s16 unk34;
    u8 pad_36[0x3E];
    s32 unk74;
    s32 unk78;
};

struct M2c_var_17_136 {
    u8 pad_0[0x34];
    s16 unk34;
    u8 pad_36[0x3E];
    s32 unk74;
    s32 unk78;
};

struct M2c_var_17_174 {
    u8 pad_0[0x34];
    s16 unk34;
    u8 pad_36[0x3E];
    s32 unk74;
    s32 unk78;
};

struct M2c_var_17_230 {
    u8 pad_0[0x34];
    s16 unk34;
    u8 pad_36[0x3E];
    s32 unk74;
    s32 unk78;
};

struct M2c_var_17_266 {
    u8 pad_0[0x20];
    s32 unk20;
    u8 pad_24[0x10];
    s16 unk34;
    u8 pad_36[0x1D];
    u8 unk53;
    u8 pad_54[0x20];
    s32 unk74;
    s32 unk78;
    u8 pad_7C[0x2A];
    s16 unkA6;
    u8 pad_A8[0x2];
};

struct M2c_var_17_316 {
    u8 pad_0[0x34];
    s16 unk34;
    u8 pad_36[0x3E];
    s32 unk74;
    s32 unk78;
};

struct M2c_var_17_346 {
    u8 pad_0[0x34];
    s16 unk34;
    u8 pad_36[0x3E];
    s32 unk74;
    s32 unk78;
};

struct M2c_var_17_376 {
    u8 pad_0[0x34];
    s16 unk34;
    u8 pad_36[0x3E];
    s32 unk74;
    s32 unk78;
};

struct M2c_var_17_422 {
    u8 pad_0[0x34];
    s16 unk34;
    u8 pad_36[0x3E];
    s32 unk74;
    s32 unk78;
    u8 pad_7C[0x40];
    s32 unkBC;
};

#endif /* RNC_GAMEPLAY_GADGETS_LOAD_HAND_GADGET_TYPES_H */
