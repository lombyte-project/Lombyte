#ifndef LOMBYTE_RNC_UI_MENUS_WEAPONS_H
#define LOMBYTE_RNC_UI_MENUS_WEAPONS_H

#include "types.h"

struct MenuScreen;

s32 count_gold_bolts(void) __asm__("FUN_00215290");
s32 count_gold_weapons(void) __asm__("FUN_00215300");
s32 draw_items_menu(struct MenuScreen *) __asm__("FUN_0021eb20");
s32 draw_selected_item_ammo(s32) __asm__("FUN_0021f158");
s32 obtain_all_gold_weapons_menu(struct MenuScreen *) __asm__("FUN_00222948");

#endif /* LOMBYTE_RNC_UI_MENUS_WEAPONS_H */
