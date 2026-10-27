#ifndef LOMBYTE_RNC_OVERLAY_L05_GAMEPLAY_ENTITIES_SHARKIGATOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L05_GAMEPLAY_ENTITIES_SHARKIGATOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l05/gameplay/entities/sharkigator.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float scenery_ship_path_advance_scale __asm__("D_L05_001613D0");
extern float scenery_ship_removal_effect_basis_scale __asm__("D_L05_001613E8");
extern float scenery_ship_removal_effect_z_addend __asm__("D_L05_001613EC");
extern float extermitank_tracked_particle_vector_a_xy_scale __asm__("D_L05_00161B20");
extern float extermitank_tracked_particle_vector_b_xy_scale __asm__("D_L05_00161B28");
extern float extermitank_tracked_particle_vector_b_spread_degrees __asm__("D_L05_00161B38");
extern float extermitank_secondary_particle_vector_a_xy_scale __asm__("D_L05_00161B50");
extern float extermitank_secondary_particle_vector_b_xy_scale __asm__("D_L05_00161B58");
extern float extermitank_secondary_particle_vector_b_spread_degrees __asm__("D_L05_00161B68");
extern float extermitank_secondary_particle_vector_a_spread_degrees __asm__("D_L05_00161B6C");
extern float extermitank_child_animation_speed_divisor __asm__("D_L05_00161AF0");
extern float extermitank_child_animation_speed_offset_scale __asm__("D_L05_00161AF4");
extern float floating_platform_rotation_y_phase_rate_degrees __asm__("D_L05_00161CD4");
extern float floating_platform_rotation_x_phase_rate_degrees __asm__("D_L05_00161CD8");
extern float floating_platform_rotation_y_amplitude_degrees __asm__("D_L05_00161CDC");
extern float floating_platform_rotation_x_amplitude_degrees __asm__("D_L05_00161CE0");
extern int floating_platform_voice_interval_min __asm__("D_L05_00161CE4");
extern int floating_platform_voice_interval_max __asm__("D_L05_00161CE8");

/* Additional scalar data read by this unit. */
extern float scenery_ship_initialization_scale_multiplier_gp __asm__("D_L05_001613D4") __attribute__((sda));
extern int extermibot_primary_spark_initial_color_gp __asm__("D_L05_00161A9C") __attribute__((sda));
extern int extermibot_primary_spark_paired_color_gp __asm__("D_L05_00161AA0") __attribute__((sda));
extern int extermibot_secondary_spark_initial_color_gp __asm__("D_L05_00161AA4") __attribute__((sda));
extern int extermibot_secondary_spark_paired_color_gp __asm__("D_L05_00161AA8") __attribute__((sda));
extern float extermibot_spark_lifetime_min_frames_gp __asm__("D_L05_00161AAC") __attribute__((sda));
extern float extermibot_spark_facing_xy_scale_gp __asm__("D_L05_00161AB8") __attribute__((sda));
extern float extermibot_spark_random_z_scale_gp __asm__("D_L05_00161ABC") __attribute__((sda));
extern float extermibot_spark_facing_vector_w_gp __asm__("D_L05_00161AC0") __attribute__((sda));
extern float extermibot_spark_random_vector_w_gp __asm__("D_L05_00161AC4") __attribute__((sda));
extern int extermibot_pre_strike_delay_frames_gp __asm__("D_L05_00161AC8") __attribute__((sda));
extern float extermibot_nonlethal_knockback_xy_scale_gp __asm__("D_L05_00161A88") __attribute__((sda));
extern float extermibot_nonlethal_knockback_z_scale_gp __asm__("D_L05_00161A8C") __attribute__((sda));
extern float extermibot_death_knockback_xy_scale_gp __asm__("D_L05_00161A90") __attribute__((sda));
extern float extermibot_death_knockback_z_scale_gp __asm__("D_L05_00161A94") __attribute__((sda));
extern float extermibot_knockback_gravity_gp __asm__("D_L05_00161AD0") __attribute__((sda));
extern float extermibot_death_knockback_gravity_gp __asm__("D_L05_00161AD4") __attribute__((sda));
extern int extermitank_tracked_particle_color_a_gp __asm__("D_L05_00161B0C") __attribute__((sda));
extern int extermitank_tracked_particle_color_b_gp __asm__("D_L05_00161B10") __attribute__((sda));
extern float extermitank_tracked_particle_lifetime_base_frames_gp __asm__("D_L05_00161B14") __attribute__((sda));
extern float extermitank_tracked_particle_vector_a_z_scale_gp __asm__("D_L05_00161B24") __attribute__((sda));
extern float extermitank_tracked_particle_vector_b_z_scale_gp __asm__("D_L05_00161B2C") __attribute__((sda));
extern float extermitank_tracked_particle_vector_a_w_gp __asm__("D_L05_00161B30") __attribute__((sda));
extern float extermitank_tracked_particle_vector_b_w_gp __asm__("D_L05_00161B34") __attribute__((sda));
extern int extermitank_secondary_particle_color_a_gp __asm__("D_L05_00161B3C") __attribute__((sda));
extern int extermitank_secondary_particle_color_b_gp __asm__("D_L05_00161B40") __attribute__((sda));
extern float extermitank_secondary_particle_lifetime_base_frames_gp __asm__("D_L05_00161B44") __attribute__((sda));
extern float extermitank_secondary_particle_vector_a_z_scale_gp __asm__("D_L05_00161B54") __attribute__((sda));
extern float extermitank_secondary_particle_vector_b_z_scale_gp __asm__("D_L05_00161B5C") __attribute__((sda));
extern float extermitank_secondary_particle_vector_a_w_gp __asm__("D_L05_00161B60") __attribute__((sda));
extern float extermitank_secondary_particle_vector_b_w_gp __asm__("D_L05_00161B64") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L05_GAMEPLAY_ENTITIES_SHARKIGATOR_GLOBALS_H */
