#ifndef LOMBYTE_RNC_UI_HUD_BANKS_H
#define LOMBYTE_RNC_UI_HUD_BANKS_H

#include "types.h"

void hud_send_texture(u32, s32, s32, s32, s32, s32) __asm__("FUN_00200b10");
void load_compressed_hud_bank(s32, s32) __asm__("FUN_00202d10");

#endif /* LOMBYTE_RNC_UI_HUD_BANKS_H */
