#ifndef LOMBYTE_RNC_UI_MENUS_FUN_0021D1F8_H
#define LOMBYTE_RNC_UI_MENUS_FUN_0021D1F8_H

#include "types.h"

struct Globals_00137B80 {
    u8 pad_0[0x1528];
    s32 unk1528;
    s32 unk152C;
};

#include "rnc/audio/music/music_stream_state.h"

struct Globals_001D5BF0 {
    u8 pad_0[0x10C];
    s32 unk10C;
};

struct MenuScreen {
    u8 pad_0[0x10];
    s32 unk10;
    u8 pad_14[0x24];
    s32 unk38;
    u8 pad_3C[0x14];
    s32 unk50;
    s32 unk54;
};

struct WordCell {
    s32 unk0;
};

#endif /* LOMBYTE_RNC_UI_MENUS_FUN_0021D1F8_H */
