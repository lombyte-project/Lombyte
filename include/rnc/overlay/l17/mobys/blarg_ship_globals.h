#ifndef LOMBYTE_RNC_OVERLAY_L17_MOBYS_BLARG_SHIP_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L17_MOBYS_BLARG_SHIP_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l17/mobys/blarg_ship.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float backdrop_shared_radius_numerator __asm__("D_L17_001623D0");
extern float backdrop_offset_radius_divisor __asm__("D_L17_001623D8");
extern float backdrop_heading_radius_numerator_addend __asm__("D_L17_001623D4");
extern float blarg_ship_firing_speed __asm__("D_L17_001623E8");

#endif /* LOMBYTE_RNC_OVERLAY_L17_MOBYS_BLARG_SHIP_GLOBALS_H */
