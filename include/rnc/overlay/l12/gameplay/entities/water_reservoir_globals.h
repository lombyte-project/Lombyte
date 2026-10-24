#ifndef LOMBYTE_RNC_OVERLAY_L12_GAMEPLAY_ENTITIES_WATER_RESERVOIR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L12_GAMEPLAY_ENTITIES_WATER_RESERVOIR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l12/gameplay/entities/water_reservoir.c. */
extern float water_reservoir_angle_rate_degrees __asm__("D_L12_001619CC") __attribute__((sda));
extern float water_reservoir_random_offset_radius __asm__("D_L12_001619D4") __attribute__((sda));
extern float water_reservoir_height_approach_rate __asm__("D_L12_00161A08") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L12_GAMEPLAY_ENTITIES_WATER_RESERVOIR_GLOBALS_H */
