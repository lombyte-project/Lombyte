#ifndef LOMBYTE_RNC_UI_HUD_SPRITES_H
#define LOMBYTE_RNC_UI_HUD_SPRITES_H

#include "types.h"

void draw_hud_rect_depth(s32, s32, s32, s32, u64, u32, s32) __asm__("FUN_00200f90");
void draw_hud_sprite_rect(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) __asm__("FUN_00200958");
void draw_hud_sprite_subpixel(s32, s32, s32, s32, s32, s32) __asm__("FUN_00200080");

#endif /* LOMBYTE_RNC_UI_HUD_SPRITES_H */
