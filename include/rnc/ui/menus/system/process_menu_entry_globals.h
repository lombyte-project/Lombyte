#ifndef LOMBYTE_RNC_UI_MENUS_SYSTEM_PROCESS_MENU_ENTRY_GLOBALS_H
#define LOMBYTE_RNC_UI_MENUS_SYSTEM_PROCESS_MENU_ENTRY_GLOBALS_H

#include "types.h"

/* Scalar data read by src/ui/menus/system/process_menu_entry.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern s32 menu_help_hud_item __asm__("D_00160070");

/* Menu entry context: [2] callback context and [3] menu of a callback
   entry, [5] help message id of the entry last shown (0: none). */
extern s32 menu_entry_context[6] __asm__("D_001993C0");

#endif /* LOMBYTE_RNC_UI_MENUS_SYSTEM_PROCESS_MENU_ENTRY_GLOBALS_H */
