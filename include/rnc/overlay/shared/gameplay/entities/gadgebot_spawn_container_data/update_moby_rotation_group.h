#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_GADGEBOT_SPAWN_CONTAINER_DATA_UPDATE_MOBY_ROTATION_GROUP_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_GADGEBOT_SPAWN_CONTAINER_DATA_UPDATE_MOBY_ROTATION_GROUP_H

#include "types.h"

/* Scalar data for src/overlays/shared/gameplay/entities/gadgebot_spawn_container.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float rotation_group_angular_velocity_adjustment_degrees_gp __asm__("D_L06_00161D38") __attribute__((sda));
extern float rotation_group_max_angular_speed_degrees_gp __asm__("D_L06_00161D40") __attribute__((sda));
extern float rotation_group_fallback_proximity_trigger_inhibit_gp __asm__("D_L06_00161D44") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_GADGEBOT_SPAWN_CONTAINER_DATA_UPDATE_MOBY_ROTATION_GROUP_H */
