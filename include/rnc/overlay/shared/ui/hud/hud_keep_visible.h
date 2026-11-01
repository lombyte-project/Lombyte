#ifndef LOMBYTE_RNC_OVERLAY_SHARED_UI_HUD_HUD_KEEP_VISIBLE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_UI_HUD_HUD_KEEP_VISIBLE_H

#include "types.h"
#include "rnc/overlay/shared/ui/hud.h"

void hud_counter_update(void) __asm__("FUN_L00_002371e0");

int  hud_screen_position(HudElem *, int *, int *) __asm__("FUN_L00_00235a70");
void apply_hud_slide_offset(HudElem *, int *, int *, int, int) __asm__("FUN_L00_00235ad8");
void initialize_numeric_hud_values_and_dimensions(HudElem *) __asm__("FUN_L00_00235c80");
void reset_numeric_hud_element(HudElem *) __asm__("FUN_L00_00235d80");
void reset_hud_animation_channels(void) __asm__("FUN_L00_00235878");
int  draw_uniform_size_hud_stars(HudElem *) __asm__("FUN_L00_00236e50");
void hud_update_particles(void) __asm__("FUN_L00_00236458");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_UI_HUD_HUD_KEEP_VISIBLE_H */
