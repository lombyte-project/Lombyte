#ifndef LOMBYTE_RNC_UI_MENUS_SAVE_DATA_SAVE_CARD_STATE_H
#define LOMBYTE_RNC_UI_MENUS_SAVE_DATA_SAVE_CARD_STATE_H

#include "types.h"

struct SaveSlotTable {
    u8 pad_0[0xD4];
    s32 unkD4;
    u8 pad_D8[0x4];
    s32 unkDC;
    u8 pad_E0[0x4];
    s32 unkE4;
};

#endif /* LOMBYTE_RNC_UI_MENUS_SAVE_DATA_SAVE_CARD_STATE_H */
