#ifndef LOMBYTE_RNC_UI_HUD_H
#define LOMBYTE_RNC_UI_HUD_H

#include "types.h"

void append_screen_rect_packet(s32, s32, s32, s32, u64, s32) __asm__("FUN_00200e08");
void update_hud(void) __asm__("FUN_001ff780");

#endif /* LOMBYTE_RNC_UI_HUD_H */
