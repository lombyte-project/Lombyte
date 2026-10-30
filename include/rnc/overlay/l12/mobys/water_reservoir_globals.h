#ifndef LOMBYTE_RNC_OVERLAY_L12_MOBYS_WATER_RESERVOIR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L12_MOBYS_WATER_RESERVOIR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l12/mobys/water_reservoir.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float water_reservoir_angle_rate_degrees __asm__("D_L12_001619CC");
extern float water_reservoir_random_offset_radius __asm__("D_L12_001619D4");
extern float water_reservoir_height_approach_rate __asm__("D_L12_00161A08");

#endif /* LOMBYTE_RNC_OVERLAY_L12_MOBYS_WATER_RESERVOIR_GLOBALS_H */
