#ifndef LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_AIRSHIP_PART_DATA_UPDATE_AIRSHIP_PART_3_H
#define LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_AIRSHIP_PART_DATA_UPDATE_AIRSHIP_PART_3_H

#include "types.h"

/* Scalar data for src/overlays/l09/gameplay/entities/airship_part.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float airship_part_initial_downward_acceleration_range_start_gp __asm__("D_L09_00161398") __attribute__((sda));
extern float airship_part_initial_downward_acceleration_range_end_gp __asm__("D_L09_0016139C") __attribute__((sda));
extern float airship_part_downward_acceleration_override_range_start_gp __asm__("D_L09_001613A0") __attribute__((sda));
extern float airship_part_downward_acceleration_override_range_end_gp __asm__("D_L09_001613A4") __attribute__((sda));
extern float airship_part_breakup_offset_velocity_scale_gp __asm__("D_L09_001613A8") __attribute__((sda));
extern float airship_part_breakup_away_from_hero_speed_gp __asm__("D_L09_001613AC") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L09_GAMEPLAY_ENTITIES_AIRSHIP_PART_DATA_UPDATE_AIRSHIP_PART_3_H */
