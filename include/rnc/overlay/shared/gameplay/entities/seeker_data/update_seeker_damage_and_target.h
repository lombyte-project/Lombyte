#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SEEKER_DATA_UPDATE_SEEKER_DAMAGE_AND_TARGET_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SEEKER_DATA_UPDATE_SEEKER_DAMAGE_AND_TARGET_H

#include "types.h"

/* Scalar data for src/overlays/shared/gameplay/entities/seeker.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float seeker_knockback_gravity_scale __asm__("D_L09_001613F8") __attribute__((sda));
extern float seeker_hurt_knockback_xy_scale __asm__("D_L09_00161400") __attribute__((sda));
extern float seeker_fatal_knockback_z_scale __asm__("D_L09_00161404") __attribute__((sda));
extern float seeker_fatal_knockback_xy_scale __asm__("D_L09_00161408") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SEEKER_DATA_UPDATE_SEEKER_DAMAGE_AND_TARGET_H */
