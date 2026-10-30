#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_BOMB_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_BOMB_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/weapons/bomb.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int   feather_base_lifetime __asm__("D_L00_001618A4");
extern float feather_scale_multiplier __asm__("D_L00_001618A0");
extern float falling_moby_horizontal_velocity_scale __asm__("D_L00_001618A8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_BOMB_GLOBALS_H */
