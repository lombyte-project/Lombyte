#ifndef LOMBYTE_RNC_OVERLAY_L14_MOBYS_FLYING_DEBRIS_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L14_MOBYS_FLYING_DEBRIS_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l14/mobys/flying_debris.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern f32 flying_debris_gravity __asm__("D_L14_001615D0");
extern f32 flying_debris_velocity_retention_factor __asm__("D_L14_001615D4");
extern f32 flying_debris_horizontal_drag_speed_threshold __asm__("D_L14_001615D8");

#endif /* LOMBYTE_RNC_OVERLAY_L14_MOBYS_FLYING_DEBRIS_GLOBALS_H */
