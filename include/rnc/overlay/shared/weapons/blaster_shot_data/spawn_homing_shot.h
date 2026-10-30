#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_BLASTER_SHOT_DATA_SPAWN_HOMING_SHOT_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_BLASTER_SHOT_DATA_SPAWN_HOMING_SHOT_H

#include "types.h"

/* Scalar data for src/overlays/shared/weapons/blaster_shot.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float homing_shot_trail_initial_size_min __asm__("D_L00_00161870") __attribute__((sda));
extern float homing_shot_trail_initial_size_max __asm__("D_L00_00161874") __attribute__((sda));
extern float homing_shot_trail_size_blend_target __asm__("D_L00_00161878") __attribute__((sda));
extern float homing_shot_attached_particle_size __asm__("D_L00_00161888") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_BLASTER_SHOT_DATA_SPAWN_HOMING_SHOT_H */
