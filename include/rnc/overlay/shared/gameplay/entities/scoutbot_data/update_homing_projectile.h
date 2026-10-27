#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SCOUTBOT_DATA_UPDATE_HOMING_PROJECTILE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SCOUTBOT_DATA_UPDATE_HOMING_PROJECTILE_H

#include "types.h"

/* Scalar data for src/overlays/shared/gameplay/entities/scoutbot.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float homing_projectile_trail_primary_motion_w_gp __asm__("D_L08_00161D1C") __attribute__((sda));
extern float homing_projectile_trail_secondary_motion_w_gp __asm__("D_L08_00161D20") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SCOUTBOT_DATA_UPDATE_HOMING_PROJECTILE_H */
