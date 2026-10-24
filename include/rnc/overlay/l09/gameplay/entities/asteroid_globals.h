#ifndef LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_ASTEROID_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_ASTEROID_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l09/gameplay/entities/asteroid.c. */
extern int textured_grid_view_settings_mode __asm__("D_L09_0015F458");
extern float platform_hero_offset_tilt_gain __asm__("D_L09_001619F4") __attribute__((sda));
extern float platform_sink_speed __asm__("D_L09_001619F8") __attribute__((sda));
extern float platform_height_return_gain __asm__("D_L09_001619FC") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_ASTEROID_GLOBALS_H */
