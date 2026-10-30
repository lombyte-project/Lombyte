#ifndef LOMBYTE_RNC_OVERLAY_L10_MOBYS_BOMBTHROWER_GRASS_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L10_MOBYS_BOMBTHROWER_GRASS_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l10/mobys/bombthrower_grass.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float bombthrower_grass_yaw_offset_limit_degrees __asm__("D_L10_00161BF8");
extern float bombthrower_grass_launch_elevation_min_degrees __asm__("D_L10_00161BFC");
extern float bombthrower_grass_launch_elevation_max_degrees __asm__("D_L10_00161C00");
extern float bombthrower_grass_launch_speed_min __asm__("D_L10_00161C04");
extern float bombthrower_grass_launch_speed_max __asm__("D_L10_00161C08");

#endif /* LOMBYTE_RNC_OVERLAY_L10_MOBYS_BOMBTHROWER_GRASS_GLOBALS_H */
