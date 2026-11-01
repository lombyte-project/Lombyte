#ifndef LOMBYTE_RNC_UI_MENUS_OPTIONS_H
#define LOMBYTE_RNC_UI_MENUS_OPTIONS_H

#include "types.h"

struct CheatsMenu;
struct MenuScreen;
struct SoundMenu;

s32 draw_cheats_menu(struct CheatsMenu *) __asm__("FUN_00221030");
s32 draw_sound_menu(struct MenuScreen *) __asm__("FUN_0021ce00");
int set_pause_sound_volume(void) __asm__("FUN_0021cb00");
s32 sound_options_menu(struct SoundMenu *) __asm__("FUN_0021cb30");

#endif /* LOMBYTE_RNC_UI_MENUS_OPTIONS_H */
