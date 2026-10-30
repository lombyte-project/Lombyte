#ifndef LOMBYTE_RNC_RENDERING_EFFECTS_UPDATE_FOG_GLOBALS_H
#define LOMBYTE_RNC_RENDERING_EFFECTS_UPDATE_FOG_GLOBALS_H

#include "types.h"

/* Scalar data read by src/rendering/view/update_fog.c. */
extern u8 water_fog_color_red __asm__("D_001610C4");
extern u8 water_fog_color_green __asm__("D_001610C5");
extern u8 water_fog_color_blue __asm__("D_001610C6");

#endif /* LOMBYTE_RNC_RENDERING_EFFECTS_UPDATE_FOG_GLOBALS_H */
