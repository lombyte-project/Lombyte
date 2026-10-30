#ifndef LOMBYTE_RNC_OVERLAY_L09_MOBYS_AIRSHIP_PART_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L09_MOBYS_AIRSHIP_PART_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l09/mobys/airship_part.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int   airship_part_low_altitude_effect_count __asm__("D_L09_00161368");
extern float airship_part_initial_tumble_x_speed_limit_degrees __asm__("D_L09_001613B0");
extern float airship_part_initial_tumble_y_speed_limit_degrees __asm__("D_L09_001613B4");
extern float airship_hull_breakup_wait_x_rotation_speed_degrees __asm__("D_L09_001613BC");

#endif /* LOMBYTE_RNC_OVERLAY_L09_MOBYS_AIRSHIP_PART_GLOBALS_H */
