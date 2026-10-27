#ifndef LOMBYTE_RNC_OVERLAY_L02_GAMEPLAY_ENTITIES_SAND_SHARK_DATA_HANDLE_SAND_SHARK_DAMAGE_AND_TARGETING_H
#define LOMBYTE_RNC_OVERLAY_L02_GAMEPLAY_ENTITIES_SAND_SHARK_DATA_HANDLE_SAND_SHARK_DAMAGE_AND_TARGETING_H

#include "types.h"

/* Scalar data for src/overlays/l02/gameplay/entities/sand_shark.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float sand_shark_death_knockback_vertical_speed_gp __asm__("D_L02_00161908") __attribute__((sda));
extern float sand_shark_death_knockback_horizontal_speed_gp __asm__("D_L02_0016190C") __attribute__((sda));
extern float sand_shark_death_knockback_gravity_gp __asm__("D_L02_00161914") __attribute__((sda));
extern float sand_shark_death_knockback_horizontal_speed_approach_rate_gp __asm__("D_L02_00161918") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L02_GAMEPLAY_ENTITIES_SAND_SHARK_DATA_HANDLE_SAND_SHARK_DAMAGE_AND_TARGETING_H */
