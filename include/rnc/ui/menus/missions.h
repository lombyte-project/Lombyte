#ifndef LOMBYTE_RNC_UI_MENUS_MISSIONS_H
#define LOMBYTE_RNC_UI_MENUS_MISSIONS_H

#include "types.h"

struct Menu;
struct MenuScreen;
struct MissionsMenu;
struct Obj;

s32 draw_mission_menu_labels(void) __asm__("FUN_0021f5f8");
s32 draw_missions_menu(struct Menu *) __asm__("FUN_0021f368");
s32 draw_missions_menu2(struct MissionsMenu *) __asm__("FUN_0021f688");
s32 enter_menu_mission_list(struct Obj *) __asm__("FUN_0021c420");
s32 update_menu_mission_list(struct MenuScreen *) __asm__("FUN_0021c4c0");

#endif /* LOMBYTE_RNC_UI_MENUS_MISSIONS_H */
