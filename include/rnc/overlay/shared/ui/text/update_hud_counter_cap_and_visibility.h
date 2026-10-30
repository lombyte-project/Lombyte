#ifndef LOMBYTE_RNC_OVERLAY_SHARED_UI_TEXT_UPDATE_HUD_COUNTER_CAP_AND_VISIBILITY_H
#define LOMBYTE_RNC_OVERLAY_SHARED_UI_TEXT_UPDATE_HUD_COUNTER_CAP_AND_VISIBILITY_H

#include "types.h"

void initialize_bolt_counter_hud(void) __asm__("FUN_L00_00239cc8");
void update_bolt_counter_hud_animation_and_visibility(void) __asm__("FUN_L00_00239d00");
void draw_bolt_counter_hud_panel(void) __asm__("FUN_L00_00239df8");
void initialize_eight_slot_hud_selector(void) __asm__("FUN_L00_00237ae8");
void update_hud_item_selector_selection_and_visibility(void) __asm__("FUN_L00_00237ba0");
void draw_hud_item_selector_panel(void) __asm__("FUN_L00_002382a0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_UI_TEXT_UPDATE_HUD_COUNTER_CAP_AND_VISIBILITY_H */
