#ifndef LOMBYTE_RNC_OVERLAY_SHARED_UI_HUD_HUD_FRAME_H
#define LOMBYTE_RNC_OVERLAY_SHARED_UI_HUD_HUD_FRAME_H

#include "types.h"

void hud_frame(void *, s32, s32, s32, s32, s32) __asm__("FUN_L00_0023b120");
void draw_rotated_hud_sprite(s32, s32, s32, s32, s32, s32) __asm__("FUN_L00_0023bac0");
void draw_cropped_hud_sprite(s32, s32, s32, s32, s32, s32) __asm__("FUN_L00_0023b6c0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_UI_HUD_HUD_FRAME_H */
