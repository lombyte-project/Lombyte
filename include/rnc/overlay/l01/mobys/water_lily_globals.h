#ifndef LOMBYTE_RNC_OVERLAY_L01_MOBYS_WATER_LILY_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L01_MOBYS_WATER_LILY_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l01/mobys/water_lily.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern f32 water_lily_spring_velocity_adjustment_scale __asm__("D_L01_00161BF0");
extern f32 water_lily_spring_damping __asm__("D_L01_00161BF4");
extern f32 water_lily_hero_separation_radius __asm__("D_L01_00161BF8");
extern f32 water_lily_max_horizontal_speed __asm__("D_L01_00161BFC");
extern f32 water_lily_collision_radius __asm__("D_L01_00161C00");

#endif /* LOMBYTE_RNC_OVERLAY_L01_MOBYS_WATER_LILY_GLOBALS_H */
