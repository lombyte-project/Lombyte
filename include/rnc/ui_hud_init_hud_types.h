#ifndef RNC_UI_HUD_INIT_HUD_TYPES_H
#define RNC_UI_HUD_INIT_HUD_TYPES_H

#include "types.h"

struct M2c_D_0015FA00 {
    u8 pad_0[0x20];
    u8 unk20;
    u8 pad_21[0x3];
};

struct M2c_D_0019A3E8 {
    s32 unk0;
    s32 unk4;
};

struct M2c_var_16_16 {
    s32 unk0;
    u8 pad_4[0x3C];
    s32 unk40;
    u8 pad_44[0x4];
    s32 unk48;
    u8 pad_4C[0xC];
    s32 unk58;
};

#endif /* RNC_UI_HUD_INIT_HUD_TYPES_H */
