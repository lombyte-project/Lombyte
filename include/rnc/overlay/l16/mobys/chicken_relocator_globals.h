#ifndef LOMBYTE_RNC_OVERLAY_L16_MOBYS_CHICKEN_RELOCATOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L16_MOBYS_CHICKEN_RELOCATOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l16/mobys/chicken_relocator.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float reposition_effect_velocity_elevation_degrees __asm__("D_L16_001618E8");
extern float reposition_effect_velocity_scale_range_start __asm__("D_L16_001618EC");
extern float reposition_effect_velocity_scale_range_end __asm__("D_L16_001618F0");
extern float reposition_effect_spawn_z_offset __asm__("D_L16_001618F4");
extern float reposition_effect_radius_range_start __asm__("D_L16_001618F8");
extern float reposition_effect_radius_range_end __asm__("D_L16_001618FC");

#endif /* LOMBYTE_RNC_OVERLAY_L16_MOBYS_CHICKEN_RELOCATOR_GLOBALS_H */
