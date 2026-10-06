#ifndef LOMBYTE_RNC_UI_HUD_INIT_HUD_H
#define LOMBYTE_RNC_UI_HUD_INIT_HUD_H

#include "types.h"

struct Globals_0015FA00 {
    u8 pad_0[0x20];
    u8 unk20;
    u8 pad_21[0x3];
};

struct Globals_0019A3E8 {
    s32 unk0;
    s32 unk4;
};

struct HudSlot {
    s32 unk0;
    u8 pad_4[0x3C];
    s32 unk40;
    u8 pad_44[0x4];
    s32 unk48;
    u8 pad_4C[0xC];
    s32 unk58;
};

#endif /* LOMBYTE_RNC_UI_HUD_INIT_HUD_H */
