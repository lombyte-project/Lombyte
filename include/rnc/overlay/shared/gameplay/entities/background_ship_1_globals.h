#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BACKGROUND_SHIP_1_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BACKGROUND_SHIP_1_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/background_ship_1.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int level_2_ship_removal_count __asm__("D_0015EDF4");
extern int level_9_ship_removal_count __asm__("D_0015EDFC");
extern float background_ship_1_path_travel_speed __asm__("D_L02_00161F64");
extern float background_ship_1_color_phase_rate_degrees __asm__("D_L02_00161F70");
extern float background_ship_2_path_travel_speed __asm__("D_L02_00161FC0");
extern float background_ship_2_color_phase_rate_degrees __asm__("D_L02_00161FC4");
extern int background_ship_2_color_tween_start __asm__("D_L02_00161FC8");
extern int background_ship_2_color_tween_end __asm__("D_L02_00161FCC");

/* Additional scalar data read by this unit. */
extern float background_ship_1_removal_effect_vector_scale_gp __asm__("D_L02_00161F7C") __attribute__((sda));
extern float background_ship_1_removal_effect_vector_z_bias_gp __asm__("D_L02_00161F80") __attribute__((sda));
extern float background_ship_2_removal_effect_vector_scale_gp __asm__("D_L02_00161FD0") __attribute__((sda));
extern float background_ship_2_removal_effect_vector_z_bias_gp __asm__("D_L02_00161FD4") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BACKGROUND_SHIP_1_GLOBALS_H */
