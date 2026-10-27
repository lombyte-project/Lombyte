#ifndef LOMBYTE_RNC_OVERLAY_L10_GAMEPLAY_ENTITIES_SCREAMER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L10_GAMEPLAY_ENTITIES_SCREAMER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l10/gameplay/entities/screamer.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int moby_group_command_frames __asm__("D_L10_00161E00");
extern float toxic_crab_hit_knockback_gravity __asm__("D_L10_00161E2C");
extern float toxic_crab_group_approach_angle_limit_degrees __asm__("D_L10_00161E48");
extern float blarg_ship_rocket_speed __asm__("D_L10_00161EE8");
extern float blarg_ship_firing_animation_speed __asm__("D_L10_00161EEC");
extern s32 directional_particle_color_range_start __asm__("D_L10_00161EF0");
extern s32 directional_particle_color_range_end __asm__("D_L10_00161EF4");
extern f32 directional_particle_speed_range_end __asm__("D_L10_00161EF8");
extern f32 directional_particle_speed_range_start __asm__("D_L10_00161EFC");
extern s32 directional_particle_variant_threshold_percent __asm__("D_L10_00161F04");
extern int purchased_nanotech_health __asm__("D_0013EDA0");

/* Additional scalar data read by this unit. */
extern int screamer_death_message_gate __asm__("D_00151F38") __attribute__((sda));
extern float toxic_crab_death_knockback_gravity_gp __asm__("D_L10_00161E28") __attribute__((sda));
extern float toxic_crab_obstacle_pause_distance_gp __asm__("D_L10_00161E34") __attribute__((sda));
extern float toxic_crab_back_away_distance_limit_gp __asm__("D_L10_00161E44") __attribute__((sda));
extern float toxic_crab_pursuit_heading_threshold_degrees_gp __asm__("D_L10_00161E4C") __attribute__((sda));
extern f32 directional_particle_gravity_gp __asm__("D_L10_00161F00") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L10_GAMEPLAY_ENTITIES_SCREAMER_GLOBALS_H */
