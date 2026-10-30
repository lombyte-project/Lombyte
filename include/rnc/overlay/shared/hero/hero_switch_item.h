#ifndef LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_SWITCH_ITEM_H
#define LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_SWITCH_ITEM_H

#include "types.h"

void stick_target(float scale, int mode) __asm__("FUN_L00_002118c8");
int  hero_slot_item_id(int i) __asm__("FUN_L00_0020d498");
void cancel_hero_item_animation(void) __asm__("FUN_L00_0020e698");
void hero_respawn(void) __asm__("FUN_L00_00211250");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_SWITCH_ITEM_H */
