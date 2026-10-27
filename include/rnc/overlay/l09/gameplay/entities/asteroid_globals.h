#ifndef LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_ASTEROID_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_ASTEROID_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l09/gameplay/entities/asteroid.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int textured_grid_view_settings_mode __asm__("D_L09_0015F458");
extern float platform_hero_offset_tilt_gain __asm__("D_L09_001619F4");
extern float platform_sink_speed __asm__("D_L09_001619F8");
extern float platform_height_return_gain __asm__("D_L09_001619FC");

#endif /* LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_ASTEROID_GLOBALS_H */
