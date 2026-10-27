#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NOVALIS_ELEVATOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NOVALIS_ELEVATOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/novalis_elevator.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern u32 cached_water_texture_id __asm__("D_L01_00161228");
extern u32 cached_water_second_texture_id __asm__("D_L01_0016122C");
extern u32 cached_water_alpha_1_fix __asm__("D_L01_00161234");
extern s32 moby_motion_particle_random_sign_base __asm__("D_L01_00161494");
extern f32 moby_motion_collision_radius __asm__("D_L01_0016149C");
extern f32 bolt_crank_radial_correction_step __asm__("D_L01_001618A0");
extern float sand_mouse_spawn_target_range __asm__("D_L01_00161904");
extern float sand_mouse_spawn_target_yaw_limit_degrees __asm__("D_L01_00161908");
extern float sand_mouse_spawn_target_pitch_limit_degrees __asm__("D_L01_0016190C");
extern float sand_mouse_spawn_offset_toward_target_distance __asm__("D_L01_00161910");
extern float sand_mouse_spawn_and_fallback_aim_height_offset __asm__("D_L01_00161914");
extern int sand_mouse_projectile_spawn_interval_frames __asm__("D_L01_00161900");
extern int amoeboid_first_movement_particle_spawn_gate_modulus_plus_one __asm__("D_L01_001619E0");
extern int amoeboid_second_movement_particle_spawn_gate_modulus_plus_one __asm__("D_L01_001619E4");
extern float amoeboid_movement_particle_max_spawn_radius __asm__("D_L01_001619E8");
extern float amoeboid_first_movement_particle_base_scale_max __asm__("D_L01_001619B8");
extern int amoeboid_first_movement_particle_lifetime_min __asm__("D_L01_001619C0");
extern int amoeboid_first_movement_particle_lifetime_max __asm__("D_L01_001619C4");
extern float amoeboid_second_movement_particle_base_scale_max __asm__("D_L01_001619D0");
extern int amoeboid_second_movement_particle_lifetime_min __asm__("D_L01_001619D8");
extern int amoeboid_second_movement_particle_lifetime_max __asm__("D_L01_001619DC");
extern s32 amoeboid_burst_group_count __asm__("D_L01_001619EC");
extern s32 amoeboid_burst_sparks_per_group __asm__("D_L01_001619F0");
extern f32 amoeboid_burst_group_position_jitter_bound __asm__("D_L01_001619F4");
extern f32 amoeboid_burst_spark_position_jitter_bound __asm__("D_L01_001619F8");
extern f32 amoeboid_burst_spark_velocity_jitter_bound __asm__("D_L01_001619FC");
extern f32 amoeboid_burst_radial_speed_min __asm__("D_L01_00161A00");
extern f32 amoeboid_burst_radial_speed_max __asm__("D_L01_00161A04");
extern f32 amoeboid_burst_vertical_speed_min __asm__("D_L01_00161A08");
extern f32 amoeboid_burst_vertical_speed_max __asm__("D_L01_00161A0C");
extern f32 amoeboid_burst_downward_velocity_adjustment_factor __asm__("D_L01_00161A10");
extern s32 amoeboid_burst_spark_duration_base __asm__("D_L01_00161A34");
extern f32 amoeboid_burst_input_motion_scale_max __asm__("D_L01_00161A38");

/* Additional scalar data read by this unit. */
extern f32 hover_target_yaw_spring_strength_degrees_gp __asm__("D_L01_00161944") __attribute__((sda));
extern f32 hover_target_yaw_damping_degrees_gp __asm__("D_L01_00161948") __attribute__((sda));
extern f32 hover_target_yaw_turn_rate_limit_degrees_gp __asm__("D_L01_0016194C") __attribute__((sda));
extern f32 hover_ground_animation_height_threshold_gp __asm__("D_L01_00161920") __attribute__((sda));
extern int sand_mouse_follow_duration_frames_gp __asm__("D_L01_00161918") __attribute__((sda));
extern int sand_mouse_follow_projectile_count_limit_gp __asm__("D_L01_0016191C") __attribute__((sda));
extern int sand_mouse_fade_duration_frames_gp __asm__("D_L01_00161924") __attribute__((sda));
extern int sand_mouse_fade_particle_random_integer_min_gp __asm__("D_L01_00161928") __attribute__((sda));
extern int sand_mouse_fade_particle_random_integer_max_gp __asm__("D_L01_0016192C") __attribute__((sda));
extern float sand_mouse_fade_particle_position_jitter_gp __asm__("D_L01_00161930") __attribute__((sda));
extern float sand_mouse_fade_particle_random_float_range_start_gp __asm__("D_L01_00161934") __attribute__((sda));
extern float sand_mouse_fade_particle_random_float_range_end_gp __asm__("D_L01_00161938") __attribute__((sda));
extern int sand_mouse_fade_particle_color_alpha_gp __asm__("D_L01_0016193C") __attribute__((sda));
extern float sand_mouse_fade_particle_time_squared_coefficient_gp __asm__("D_L01_00161940") __attribute__((sda));
extern float large_amoeboid_path_lunge_distance_threshold_gp __asm__("D_L01_001619AC") __attribute__((sda));
extern float amoeboid_first_movement_particle_secondary_sample_max_gp __asm__("D_L01_001619BC") __attribute__((sda));
extern int amoeboid_first_movement_particle_header_and_data_value_gp __asm__("D_L01_001619B0") __attribute__((sda));
extern int amoeboid_first_movement_particle_data_value_gp __asm__("D_L01_001619B4") __attribute__((sda));
extern int amoeboid_second_movement_particle_header_and_data_value_gp __asm__("D_L01_001619C8") __attribute__((sda));
extern int amoeboid_second_movement_particle_data_value_gp __asm__("D_L01_001619CC") __attribute__((sda));
extern float amoeboid_second_movement_particle_secondary_sample_max_gp __asm__("D_L01_001619D4") __attribute__((sda));
extern s32 amoeboid_burst_spark_spawn_color_randomization_start_gp __asm__("D_L01_00161A1C") __attribute__((sda));
extern s32 amoeboid_burst_spark_secondary_color_randomization_start_gp __asm__("D_L01_00161A20") __attribute__((sda));
extern s32 amoeboid_burst_spark_spawn_color_randomization_end_gp __asm__("D_L01_00161A24") __attribute__((sda));
extern s32 amoeboid_burst_spark_secondary_color_randomization_end_gp __asm__("D_L01_00161A28") __attribute__((sda));
extern s32 amoeboid_burst_downward_velocity_adjustment_frames_gp __asm__("D_L01_00161A30") __attribute__((sda));
extern s32 gunship_path_updates_disabled __asm__("D_L01_00161B00");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NOVALIS_ELEVATOR_GLOBALS_H */
