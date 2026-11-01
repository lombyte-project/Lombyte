#ifndef LOMBYTE_RNC_UI_MENUS_SELECTION_H
#define LOMBYTE_RNC_UI_MENUS_SELECTION_H

#include "types.h"

struct MenuScreen;

s32  build_filtered_menu_item_list(void) __asm__("FUN_002216c0");
s32  handle_cyclic_menu_selection_input(struct MenuScreen *) __asm__("FUN_00221d68");
void load_selection_list(struct MenuScreen *, s32) __asm__("FUN_0021c9c8");
s32  update_menu_cycle_selection(struct MenuScreen *) __asm__("FUN_00221e50");
s32  update_menu_grid_selection(struct MenuScreen *) __asm__("FUN_0021b858");
int  update_menu_icon_list(struct MenuScreen *) __asm__("FUN_00219e10");
s32  update_menu_item_slots(struct MenuScreen *) __asm__("FUN_0021c7a0");
s32  update_menu_option_list(struct MenuScreen *) __asm__("FUN_00220e28");

#endif /* LOMBYTE_RNC_UI_MENUS_SELECTION_H */
