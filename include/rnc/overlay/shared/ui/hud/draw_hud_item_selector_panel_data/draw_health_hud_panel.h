#ifndef LOMBYTE_RNC_OVERLAY_SHARED_UI_HUD_DRAW_HUD_ITEM_SELECTOR_PANEL_DATA_DRAW_HEALTH_HUD_PANEL_H
#define LOMBYTE_RNC_OVERLAY_SHARED_UI_HUD_DRAW_HUD_ITEM_SELECTOR_PANEL_DATA_DRAW_HEALTH_HUD_PANEL_H

#include "types.h"

/* Scalar data for src/overlays/shared/ui/hud/draw_hud_item_selector_panel.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern s32 health_hud_panel_max_alpha __asm__("D_L00_0015F820") __attribute__((sda));
extern s32 health_hud_panel_y_offset __asm__("D_L00_0015F810") __attribute__((sda));
extern s32 health_hud_panel_row_y_step __asm__("D_L00_0015F80C") __attribute__((sda));
extern s32 health_hud_panel_cap_size __asm__("D_L00_0015F81C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_UI_HUD_DRAW_HUD_ITEM_SELECTOR_PANEL_DATA_DRAW_HEALTH_HUD_PANEL_H */
