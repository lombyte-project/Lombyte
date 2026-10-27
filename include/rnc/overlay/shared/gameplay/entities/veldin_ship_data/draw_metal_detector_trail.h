#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_VELDIN_SHIP_DATA_DRAW_METAL_DETECTOR_TRAIL_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_VELDIN_SHIP_DATA_DRAW_METAL_DETECTOR_TRAIL_H

#include "types.h"

/* Scalar data for src/overlays/shared/gameplay/entities/veldin_ship.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int metal_detector_trail_blend_minuend_selector_gp __asm__("D_L00_00161900") __attribute__((sda));
extern int metal_detector_trail_blend_subtrahend_selector_gp __asm__("D_L00_00161904") __attribute__((sda));
extern int metal_detector_trail_blend_alpha_factor_selector_gp __asm__("D_L00_00161908") __attribute__((sda));
extern int metal_detector_trail_blend_addend_selector_gp __asm__("D_L00_0016190C") __attribute__((sda));
extern int metal_detector_trail_blend_fixed_alpha_gp __asm__("D_L00_00161910") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_VELDIN_SHIP_DATA_DRAW_METAL_DETECTOR_TRAIL_H */
