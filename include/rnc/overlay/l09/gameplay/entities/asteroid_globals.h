#ifndef LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_ASTEROID_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_ASTEROID_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l09/gameplay/entities/asteroid.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int textured_grid_view_settings_mode __asm__("D_L09_0015F458");
extern float platform_hero_offset_tilt_gain __asm__("D_L09_001619F4");
extern float platform_sink_speed __asm__("D_L09_001619F8");
extern float platform_height_return_gain __asm__("D_L09_001619FC");

/* Additional scalar data read by this unit. */
extern float saved_textured_grid_fog_near_distance __asm__("D_L09_001612F8");
extern float saved_textured_grid_fog_far_distance __asm__("D_L09_001612FC");
extern float saved_textured_grid_fog_near_intensity __asm__("D_L09_00161300");
extern float platform_active_tilt_error_gain_gp __asm__("D_L09_00161A04") __attribute__((sda));
extern float platform_active_tilt_velocity_damping_gp __asm__("D_L09_00161A08") __attribute__((sda));
extern float platform_active_tilt_velocity_limit_gp __asm__("D_L09_00161A0C") __attribute__((sda));
extern float platform_return_tilt_error_gain_gp __asm__("D_L09_00161A10") __attribute__((sda));
extern float platform_return_tilt_velocity_damping_gp __asm__("D_L09_00161A14") __attribute__((sda));
extern float platform_return_tilt_velocity_limit_gp __asm__("D_L09_00161A18") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_ASTEROID_GLOBALS_H */
