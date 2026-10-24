#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_GADGEBOT_SPAWN_CONTAINER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_GADGEBOT_SPAWN_CONTAINER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/gadgebot_spawn_container.c. */
extern float frog_facing_rotation_blend_factor __asm__("D_L06_00161C00") __attribute__((sda));
extern s32 moby_spark_burst_count __asm__("D_L06_00161C18") __attribute__((sda));
extern s32 moby_sparks_per_burst __asm__("D_L06_00161C1C") __attribute__((sda));
extern f32 moby_spark_burst_position_jitter_bound __asm__("D_L06_00161C20") __attribute__((sda));
extern f32 moby_spark_position_jitter_bound __asm__("D_L06_00161C24") __attribute__((sda));
extern f32 moby_spark_velocity_jitter_bound __asm__("D_L06_00161C28") __attribute__((sda));
extern f32 moby_spark_horizontal_speed_range_start __asm__("D_L06_00161C2C") __attribute__((sda));
extern f32 moby_spark_horizontal_speed_range_end __asm__("D_L06_00161C30") __attribute__((sda));
extern f32 moby_spark_vertical_speed_range_start __asm__("D_L06_00161C34") __attribute__((sda));
extern f32 moby_spark_vertical_speed_range_end __asm__("D_L06_00161C38") __attribute__((sda));
extern f32 moby_spark_push_scale_range_end __asm__("D_L06_00161C64") __attribute__((sda));
extern s32 gadgebot_target_cache_frame __asm__("D_L06_00161CE8") __attribute__((sda));
extern s32 cached_gadgebot_target_count __asm__("D_L06_00161CE4") __attribute__((sda));
extern float rotation_group_position_offset_distance __asm__("D_L06_00161D48") __attribute__((sda));
extern int energy_sphere_holder_beam_color_start __asm__("D_L06_00161E58") __attribute__((sda));
extern int energy_sphere_holder_beam_color_end __asm__("D_L06_00161E5C") __attribute__((sda));
extern int energy_sphere_holder_beam_texture_index __asm__("D_L06_00161E64") __attribute__((sda));
extern int energy_sphere_holder_spark_color __asm__("D_L06_00161E68") __attribute__((sda));
extern float energy_sphere_holder_spark_size_scale __asm__("D_L06_00161E6C") __attribute__((sda));
extern int energy_sphere_holder_spark_lifetime_frames __asm__("D_L06_00161E70") __attribute__((sda));
extern float energy_sphere_rotation_rate_x_degrees __asm__("D_L06_00161E74") __attribute__((sda));
extern float energy_sphere_rotation_rate_y_degrees __asm__("D_L06_00161E78") __attribute__((sda));
extern float energy_sphere_rotation_rate_z_degrees __asm__("D_L06_00161E7C") __attribute__((sda));
extern float energy_sphere_destruction_speed_range_start __asm__("D_L06_00161E84") __attribute__((sda));
extern float energy_sphere_destruction_speed_range_end __asm__("D_L06_00161E88") __attribute__((sda));
extern int energy_sphere_destruction_nonzero_branch_color_start __asm__("D_L06_00161E98") __attribute__((sda));
extern int energy_sphere_destruction_nonzero_branch_color_end __asm__("D_L06_00161E9C") __attribute__((sda));
extern int energy_sphere_destruction_zero_branch_color_start __asm__("D_L06_00161EA0") __attribute__((sda));
extern int energy_sphere_destruction_zero_branch_color_end __asm__("D_L06_00161EA4") __attribute__((sda));
extern float energy_sphere_destruction_scale_range_start __asm__("D_L06_00161EA8") __attribute__((sda));
extern float energy_sphere_destruction_scale_range_end __asm__("D_L06_00161EAC") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_GADGEBOT_SPAWN_CONTAINER_GLOBALS_H */
