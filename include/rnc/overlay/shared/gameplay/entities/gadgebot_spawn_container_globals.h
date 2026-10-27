#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_GADGEBOT_SPAWN_CONTAINER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_GADGEBOT_SPAWN_CONTAINER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/gadgebot_spawn_container.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float frog_facing_rotation_blend_factor __asm__("D_L06_00161C00");
extern s32 moby_spark_burst_count __asm__("D_L06_00161C18");
extern s32 moby_sparks_per_burst __asm__("D_L06_00161C1C");
extern f32 moby_spark_burst_position_jitter_bound __asm__("D_L06_00161C20");
extern f32 moby_spark_position_jitter_bound __asm__("D_L06_00161C24");
extern f32 moby_spark_velocity_jitter_bound __asm__("D_L06_00161C28");
extern f32 moby_spark_horizontal_speed_range_start __asm__("D_L06_00161C2C");
extern f32 moby_spark_horizontal_speed_range_end __asm__("D_L06_00161C30");
extern f32 moby_spark_vertical_speed_range_start __asm__("D_L06_00161C34");
extern f32 moby_spark_vertical_speed_range_end __asm__("D_L06_00161C38");
extern f32 moby_spark_push_scale_range_end __asm__("D_L06_00161C64");
extern s32 gadgebot_target_cache_frame __asm__("D_L06_00161CE8");
extern s32 cached_gadgebot_target_count __asm__("D_L06_00161CE4");
extern float rotation_group_position_offset_distance __asm__("D_L06_00161D48");
extern int energy_sphere_holder_beam_color_start __asm__("D_L06_00161E58");
extern int energy_sphere_holder_beam_color_end __asm__("D_L06_00161E5C");
extern int energy_sphere_holder_beam_texture_index __asm__("D_L06_00161E64");
extern int energy_sphere_holder_spark_color __asm__("D_L06_00161E68");
extern float energy_sphere_holder_spark_size_scale __asm__("D_L06_00161E6C");
extern int energy_sphere_holder_spark_lifetime_frames __asm__("D_L06_00161E70");
extern float energy_sphere_rotation_rate_x_degrees __asm__("D_L06_00161E74");
extern float energy_sphere_rotation_rate_y_degrees __asm__("D_L06_00161E78");
extern float energy_sphere_rotation_rate_z_degrees __asm__("D_L06_00161E7C");
extern float energy_sphere_destruction_speed_range_start __asm__("D_L06_00161E84");
extern float energy_sphere_destruction_speed_range_end __asm__("D_L06_00161E88");
extern int energy_sphere_destruction_nonzero_branch_color_start __asm__("D_L06_00161E98");
extern int energy_sphere_destruction_nonzero_branch_color_end __asm__("D_L06_00161E9C");
extern int energy_sphere_destruction_zero_branch_color_start __asm__("D_L06_00161EA0");
extern int energy_sphere_destruction_zero_branch_color_end __asm__("D_L06_00161EA4");
extern float energy_sphere_destruction_scale_range_start __asm__("D_L06_00161EA8");
extern float energy_sphere_destruction_scale_range_end __asm__("D_L06_00161EAC");

/* Additional scalar data read by this unit. */
extern f32 frog_glow_phase_rate_degrees_gp __asm__("D_L06_00161C04") __attribute__((sda));
extern int frog_glow_color_start_gp __asm__("D_L06_00161C08") __attribute__((sda));
extern int frog_glow_color_end_gp __asm__("D_L06_00161C0C") __attribute__((sda));
extern f32 moby_spark_vertical_velocity_correction_scale_gp __asm__("D_L06_00161C3C") __attribute__((sda));
extern s32 moby_spark_first_color_range_start_gp __asm__("D_L06_00161C48") __attribute__((sda));
extern s32 moby_spark_second_color_range_start_gp __asm__("D_L06_00161C4C") __attribute__((sda));
extern s32 moby_spark_first_color_range_end_gp __asm__("D_L06_00161C50") __attribute__((sda));
extern s32 moby_spark_second_color_range_end_gp __asm__("D_L06_00161C54") __attribute__((sda));
extern s32 moby_spark_first_frame_count_base_gp __asm__("D_L06_00161C58") __attribute__((sda));
extern s32 moby_spark_second_frame_count_base_gp __asm__("D_L06_00161C5C") __attribute__((sda));
extern s32 moby_spark_third_frame_count_base_gp __asm__("D_L06_00161C60") __attribute__((sda));
extern float gadgebot_position_snapshot_z_offset_gp __asm__("D_L06_00161CE0") __attribute__((sda));
extern float rotation_group_angular_deceleration_degrees_gp __asm__("D_L06_00161D3C") __attribute__((sda));
extern float rotation_group_angular_velocity_adjustment_degrees_gp __asm__("D_L06_00161D38") __attribute__((sda));
extern float rotation_group_max_angular_speed_degrees_gp __asm__("D_L06_00161D40") __attribute__((sda));
extern float rotation_group_fallback_proximity_trigger_inhibit_gp __asm__("D_L06_00161D44") __attribute__((sda));
extern int energy_sphere_holder_beam_particle_variant_flag_gp __asm__("D_L06_00161E60") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_GADGEBOT_SPAWN_CONTAINER_GLOBALS_H */
