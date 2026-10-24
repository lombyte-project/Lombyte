#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BOMB_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BOMB_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/bomb.c. */
extern int feather_base_lifetime __asm__("D_L00_001618A4") __attribute__((sda));
extern float feather_scale_multiplier __asm__("D_L00_001618A0") __attribute__((sda));
extern float falling_moby_horizontal_velocity_scale __asm__("D_L00_001618A8") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BOMB_GLOBALS_H */
