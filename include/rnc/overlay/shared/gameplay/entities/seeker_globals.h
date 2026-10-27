#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SEEKER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SEEKER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/seeker.c. */
extern s32 seeker_state_dispatch_mode __asm__("D_L09_0015F5C4");

/* Additional scalar data read by this unit. */
extern float seeker_knockback_gravity_scale __asm__("D_L09_001613F8") __attribute__((sda));
extern float seeker_hurt_knockback_xy_scale __asm__("D_L09_00161400") __attribute__((sda));
extern float seeker_fatal_knockback_z_scale __asm__("D_L09_00161404") __attribute__((sda));
extern float seeker_fatal_knockback_xy_scale __asm__("D_L09_00161408") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SEEKER_GLOBALS_H */
