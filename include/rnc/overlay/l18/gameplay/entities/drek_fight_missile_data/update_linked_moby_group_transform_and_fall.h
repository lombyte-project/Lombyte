#ifndef LOMBYTE_RNC_OVERLAY_L18_GAMEPLAY_ENTITIES_DREK_FIGHT_MISSILE_DATA_UPDATE_LINKED_MOBY_GROUP_TRANSFORM_AND_FALL_H
#define LOMBYTE_RNC_OVERLAY_L18_GAMEPLAY_ENTITIES_DREK_FIGHT_MISSILE_DATA_UPDATE_LINKED_MOBY_GROUP_TRANSFORM_AND_FALL_H

#include "types.h"

/* Scalar data for src/overlays/l18/gameplay/entities/drek_fight_missile.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int group_transform_advance_enabled_gp __asm__("D_L18_00162288") __attribute__((sda));
extern float group_transform_initial_acceleration_gp __asm__("D_L18_00162290") __attribute__((sda));
extern float group_transform_initial_braking_gp __asm__("D_L18_00162294") __attribute__((sda));
extern float group_transform_initial_maximum_speed_gp __asm__("D_L18_00162298") __attribute__((sda));
extern float group_transform_later_acceleration_gp __asm__("D_L18_0016229C") __attribute__((sda));
extern float group_transform_later_braking_gp __asm__("D_L18_001622A0") __attribute__((sda));
extern float group_transform_later_maximum_speed_gp __asm__("D_L18_001622A4") __attribute__((sda));
extern float group_transform_fall_progress_threshold_gp __asm__("D_L18_001622B4") __attribute__((sda));
extern float group_transform_debris_start_progress_threshold_gp __asm__("D_L18_001622B8") __attribute__((sda));
extern int group_transform_debris_initial_random_bound_gp __asm__("D_L18_001622BC") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L18_GAMEPLAY_ENTITIES_DREK_FIGHT_MISSILE_DATA_UPDATE_LINKED_MOBY_GROUP_TRANSFORM_AND_FALL_H */
