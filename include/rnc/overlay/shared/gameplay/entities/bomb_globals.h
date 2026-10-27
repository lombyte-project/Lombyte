#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BOMB_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BOMB_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/bomb.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int feather_base_lifetime __asm__("D_L00_001618A4");
extern float feather_scale_multiplier __asm__("D_L00_001618A0");
extern float falling_moby_horizontal_velocity_scale __asm__("D_L00_001618A8");

/* Additional scalar data read by this unit. */
extern float homing_shot_trail_initial_size_min __asm__("D_L00_00161870") __attribute__((sda));
extern float homing_shot_trail_initial_size_max __asm__("D_L00_00161874") __attribute__((sda));
extern float homing_shot_trail_size_blend_target __asm__("D_L00_00161878") __attribute__((sda));
extern float homing_shot_attached_particle_size __asm__("D_L00_00161888") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BOMB_GLOBALS_H */
