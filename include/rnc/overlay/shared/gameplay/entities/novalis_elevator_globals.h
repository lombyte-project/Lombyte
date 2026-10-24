#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NOVALIS_ELEVATOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NOVALIS_ELEVATOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/novalis_elevator.c. */
extern u32 cached_water_texture_id __asm__("D_L01_00161228") __attribute__((sda));
extern u32 cached_water_second_texture_id __asm__("D_L01_0016122C") __attribute__((sda));
extern u32 cached_water_alpha_1_fix __asm__("D_L01_00161234") __attribute__((sda));
extern s32 moby_motion_particle_random_sign_base __asm__("D_L01_00161494") __attribute__((sda));
extern f32 moby_motion_collision_radius __asm__("D_L01_0016149C") __attribute__((sda));
extern f32 bolt_crank_radial_correction_step __asm__("D_L01_001618A0") __attribute__((sda));
extern float sand_mouse_spawn_target_range __asm__("D_L01_00161904") __attribute__((sda));
extern float sand_mouse_spawn_target_yaw_limit_degrees __asm__("D_L01_00161908") __attribute__((sda));
extern float sand_mouse_spawn_target_pitch_limit_degrees __asm__("D_L01_0016190C") __attribute__((sda));
extern float sand_mouse_spawn_offset_toward_target_distance __asm__("D_L01_00161910") __attribute__((sda));
extern float sand_mouse_spawn_and_fallback_aim_height_offset __asm__("D_L01_00161914") __attribute__((sda));
extern int sand_mouse_projectile_spawn_interval_frames __asm__("D_L01_00161900") __attribute__((sda));
extern int amoeboid_first_movement_particle_spawn_gate_modulus_plus_one __asm__("D_L01_001619E0") __attribute__((sda));
extern int amoeboid_second_movement_particle_spawn_gate_modulus_plus_one __asm__("D_L01_001619E4") __attribute__((sda));
extern float amoeboid_movement_particle_max_spawn_radius __asm__("D_L01_001619E8") __attribute__((sda));
extern float amoeboid_first_movement_particle_base_scale_max __asm__("D_L01_001619B8") __attribute__((sda));
extern int amoeboid_first_movement_particle_lifetime_min __asm__("D_L01_001619C0") __attribute__((sda));
extern int amoeboid_first_movement_particle_lifetime_max __asm__("D_L01_001619C4") __attribute__((sda));
extern float amoeboid_second_movement_particle_base_scale_max __asm__("D_L01_001619D0") __attribute__((sda));
extern int amoeboid_second_movement_particle_lifetime_min __asm__("D_L01_001619D8") __attribute__((sda));
extern int amoeboid_second_movement_particle_lifetime_max __asm__("D_L01_001619DC") __attribute__((sda));
extern s32 amoeboid_burst_group_count __asm__("D_L01_001619EC") __attribute__((sda));
extern s32 amoeboid_burst_sparks_per_group __asm__("D_L01_001619F0") __attribute__((sda));
extern f32 amoeboid_burst_group_position_jitter_bound __asm__("D_L01_001619F4") __attribute__((sda));
extern f32 amoeboid_burst_spark_position_jitter_bound __asm__("D_L01_001619F8") __attribute__((sda));
extern f32 amoeboid_burst_spark_velocity_jitter_bound __asm__("D_L01_001619FC") __attribute__((sda));
extern f32 amoeboid_burst_radial_speed_min __asm__("D_L01_00161A00") __attribute__((sda));
extern f32 amoeboid_burst_radial_speed_max __asm__("D_L01_00161A04") __attribute__((sda));
extern f32 amoeboid_burst_vertical_speed_min __asm__("D_L01_00161A08") __attribute__((sda));
extern f32 amoeboid_burst_vertical_speed_max __asm__("D_L01_00161A0C") __attribute__((sda));
extern f32 amoeboid_burst_downward_velocity_adjustment_factor __asm__("D_L01_00161A10") __attribute__((sda));
extern s32 amoeboid_burst_spark_duration_base __asm__("D_L01_00161A34") __attribute__((sda));
extern f32 amoeboid_burst_input_motion_scale_max __asm__("D_L01_00161A38") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NOVALIS_ELEVATOR_GLOBALS_H */
