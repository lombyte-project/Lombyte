#ifndef LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_DEBUG_UPDATE_DEBUG_MENU_H
#define LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_DEBUG_UPDATE_DEBUG_MENU_H

#include "types.h"

void update_debug_menu(void) __asm__("FUN_L00_001f4598");
void draw_debug_menu(void) __asm__("FUN_L00_001f58e0");
void flush_debug_text_queue(void) __asm__("FUN_L00_001f36f8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_DEBUG_UPDATE_DEBUG_MENU_H */
