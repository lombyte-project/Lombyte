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

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NOVALIS_ELEVATOR_GLOBALS_H */
