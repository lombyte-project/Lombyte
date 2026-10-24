#ifndef LOMBYTE_RNC_OVERLAY_L10_GAMEPLAY_ENTITIES_SCREAMER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L10_GAMEPLAY_ENTITIES_SCREAMER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l10/gameplay/entities/screamer.c. */
extern int moby_group_command_frames __asm__("D_L10_00161E00") __attribute__((sda));
extern float toxic_crab_hit_knockback_gravity __asm__("D_L10_00161E2C") __attribute__((sda));
extern float toxic_crab_group_approach_angle_limit_degrees __asm__("D_L10_00161E48") __attribute__((sda));
extern float blarg_ship_rocket_speed __asm__("D_L10_00161EE8") __attribute__((sda));
extern float blarg_ship_firing_animation_speed __asm__("D_L10_00161EEC") __attribute__((sda));
extern s32 directional_particle_color_range_start __asm__("D_L10_00161EF0") __attribute__((sda));
extern s32 directional_particle_color_range_end __asm__("D_L10_00161EF4") __attribute__((sda));
extern f32 directional_particle_speed_range_end __asm__("D_L10_00161EF8") __attribute__((sda));
extern f32 directional_particle_speed_range_start __asm__("D_L10_00161EFC") __attribute__((sda));
extern s32 directional_particle_variant_threshold_percent __asm__("D_L10_00161F04") __attribute__((sda));
extern int purchased_nanotech_health __asm__("D_0013EDA0") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L10_GAMEPLAY_ENTITIES_SCREAMER_GLOBALS_H */
