#ifndef LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_AIRSHIP_PART_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_AIRSHIP_PART_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l09/gameplay/entities/airship_part.c. */
extern int airship_part_low_altitude_effect_count __asm__("D_L09_00161368") __attribute__((sda));
extern float airship_part_initial_tumble_x_speed_limit_degrees __asm__("D_L09_001613B0") __attribute__((sda));
extern float airship_part_initial_tumble_y_speed_limit_degrees __asm__("D_L09_001613B4") __attribute__((sda));
extern float airship_hull_breakup_wait_x_rotation_speed_degrees __asm__("D_L09_001613BC") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_AIRSHIP_PART_GLOBALS_H */
