#ifndef LOMBYTE_RNC_OVERLAY_L12_GAMEPLAY_ENTITIES_WATER_RESERVOIR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L12_GAMEPLAY_ENTITIES_WATER_RESERVOIR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l12/gameplay/entities/water_reservoir.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float water_reservoir_angle_rate_degrees __asm__("D_L12_001619CC");
extern float water_reservoir_random_offset_radius __asm__("D_L12_001619D4");
extern float water_reservoir_height_approach_rate __asm__("D_L12_00161A08");

/* Additional scalar data read by this unit. */
extern float water_reservoir_effect_delta_fourth_component_range_start_gp __asm__("D_L12_001619E0") __attribute__((sda));
extern float water_reservoir_effect_delta_fourth_component_range_end_gp __asm__("D_L12_001619E4") __attribute__((sda));
extern int water_reservoir_effect_delta_scale_divisor_gp __asm__("D_L12_001619EC") __attribute__((sda));
extern float water_reservoir_unmirrored_effect_spawn_z_offset_gp __asm__("D_L12_001619FC") __attribute__((sda));
extern float water_reservoir_effect_delta_z_reference_gp __asm__("D_L12_00161A00") __attribute__((sda));
extern float water_reservoir_mirrored_effect_spawn_z_offset_gp __asm__("D_L12_00161A04") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L12_GAMEPLAY_ENTITIES_WATER_RESERVOIR_GLOBALS_H */
