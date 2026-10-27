#ifndef LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_AIRSHIP_PART_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_AIRSHIP_PART_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l09/gameplay/entities/airship_part.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int airship_part_low_altitude_effect_count __asm__("D_L09_00161368");
extern float airship_part_initial_tumble_x_speed_limit_degrees __asm__("D_L09_001613B0");
extern float airship_part_initial_tumble_y_speed_limit_degrees __asm__("D_L09_001613B4");
extern float airship_hull_breakup_wait_x_rotation_speed_degrees __asm__("D_L09_001613BC");

/* Additional scalar data read by this unit. */
extern float airship_part_initial_downward_acceleration_range_start_gp __asm__("D_L09_00161398") __attribute__((sda));
extern float airship_part_initial_downward_acceleration_range_end_gp __asm__("D_L09_0016139C") __attribute__((sda));
extern float airship_part_downward_acceleration_override_range_start_gp __asm__("D_L09_001613A0") __attribute__((sda));
extern float airship_part_downward_acceleration_override_range_end_gp __asm__("D_L09_001613A4") __attribute__((sda));
extern float airship_part_breakup_offset_velocity_scale_gp __asm__("D_L09_001613A8") __attribute__((sda));
extern float airship_part_breakup_away_from_hero_speed_gp __asm__("D_L09_001613AC") __attribute__((sda));
extern float airship_part_breakup_away_from_hero_speed_override_gp __asm__("D_L09_001613B8") __attribute__((sda));
extern int airship_part_breakup_delay_range_start_gp __asm__("D_L09_001613C0") __attribute__((sda));
extern int airship_part_breakup_delay_range_end_gp __asm__("D_L09_001613C4") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_AIRSHIP_PART_GLOBALS_H */
