#ifndef LOMBYTE_RNC_UI_MAP_LEVEL_SELECTION_H
#define LOMBYTE_RNC_UI_MAP_LEVEL_SELECTION_H

#include "types.h"

struct MenuScreen;

void draw_level_selection_map(s32, s32, s32, s32) __asm__("FUN_001fd748");
s32  draw_level_selection_map_screen(struct MenuScreen *) __asm__("FUN_00221930");
int  update_level_selection_map_screen(void) __asm__("FUN_00221908");

#endif /* LOMBYTE_RNC_UI_MAP_LEVEL_SELECTION_H */
