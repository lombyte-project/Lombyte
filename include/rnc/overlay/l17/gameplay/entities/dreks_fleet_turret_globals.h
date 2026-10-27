#ifndef LOMBYTE_RNC_OVERLAY_L17_GAMEPLAY_ENTITIES_DREKS_FLEET_TURRET_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L17_GAMEPLAY_ENTITIES_DREKS_FLEET_TURRET_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l17/gameplay/entities/dreks_fleet_turret.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float fleet_radial_uv_scroll_rate __asm__("D_L17_00161BBC");
extern int fleet_radial_alpha_base_value __asm__("D_L17_00161B98");
extern int fleet_radial_quad_rgba __asm__("D_L17_00161BB8");

/* Additional scalar data read by this unit. */
extern int fleet_radial_blend_subtract_color_selector __asm__("D_L17_00161B9C") __attribute__((sda));
extern int fleet_radial_blend_alpha_factor_selector __asm__("D_L17_00161BA0") __attribute__((sda));
extern int fleet_radial_blend_add_color_selector __asm__("D_L17_00161BA4") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L17_GAMEPLAY_ENTITIES_DREKS_FLEET_TURRET_GLOBALS_H */
