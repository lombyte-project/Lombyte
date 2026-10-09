#ifndef LOMBYTE_RNC_UI_MENUS_PANEL_SLOTS_H
#define LOMBYTE_RNC_UI_MENUS_PANEL_SLOTS_H

#include "types.h"

/* The 14 menu mobys, one per panel slot: every loop over them (create in FUN_00218f98,
   delete in FUN_002191b8, draw in FUN_002196b8, page switch in FUN_002192a8) runs to 14. */
extern void *panel_slots[14] __asm__("D_001D5D90");

#endif /* LOMBYTE_RNC_UI_MENUS_PANEL_SLOTS_H */
