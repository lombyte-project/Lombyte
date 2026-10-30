#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_GADGEBOT_SPAWN_CONTAINER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_GADGEBOT_SPAWN_CONTAINER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/enemies/gadgebot_spawn_container.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float frog_facing_rotation_blend_factor __asm__("D_L06_00161C00");
extern s32   moby_spark_burst_count __asm__("D_L06_00161C18");
extern s32   moby_sparks_per_burst __asm__("D_L06_00161C1C");
extern f32   moby_spark_burst_position_jitter_bound __asm__("D_L06_00161C20");
extern f32   moby_spark_position_jitter_bound __asm__("D_L06_00161C24");
extern f32   moby_spark_velocity_jitter_bound __asm__("D_L06_00161C28");
extern f32   moby_spark_horizontal_speed_range_start __asm__("D_L06_00161C2C");
extern f32   moby_spark_horizontal_speed_range_end __asm__("D_L06_00161C30");
extern f32   moby_spark_vertical_speed_range_start __asm__("D_L06_00161C34");
extern f32   moby_spark_vertical_speed_range_end __asm__("D_L06_00161C38");
extern f32   moby_spark_push_scale_range_end __asm__("D_L06_00161C64");
extern s32   gadgebot_target_cache_frame __asm__("D_L06_00161CE8");
extern s32   cached_gadgebot_target_count __asm__("D_L06_00161CE4");
extern float rotation_group_position_offset_distance __asm__("D_L06_00161D48");
extern int   energy_sphere_holder_beam_color_start __asm__("D_L06_00161E58");
extern int   energy_sphere_holder_beam_color_end __asm__("D_L06_00161E5C");
extern int   energy_sphere_holder_beam_texture_index __asm__("D_L06_00161E64");
extern int   energy_sphere_holder_spark_color __asm__("D_L06_00161E68");
extern float energy_sphere_holder_spark_size_scale __asm__("D_L06_00161E6C");
extern int   energy_sphere_holder_spark_lifetime_frames __asm__("D_L06_00161E70");
extern float energy_sphere_rotation_rate_x_degrees __asm__("D_L06_00161E74");
extern float energy_sphere_rotation_rate_y_degrees __asm__("D_L06_00161E78");
extern float energy_sphere_rotation_rate_z_degrees __asm__("D_L06_00161E7C");
extern float energy_sphere_destruction_speed_range_start __asm__("D_L06_00161E84");
extern float energy_sphere_destruction_speed_range_end __asm__("D_L06_00161E88");
extern int   energy_sphere_destruction_nonzero_branch_color_start __asm__("D_L06_00161E98");
extern int   energy_sphere_destruction_nonzero_branch_color_end __asm__("D_L06_00161E9C");
extern int   energy_sphere_destruction_zero_branch_color_start __asm__("D_L06_00161EA0");
extern int   energy_sphere_destruction_zero_branch_color_end __asm__("D_L06_00161EA4");
extern float energy_sphere_destruction_scale_range_start __asm__("D_L06_00161EA8");
extern float energy_sphere_destruction_scale_range_end __asm__("D_L06_00161EAC");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_GADGEBOT_SPAWN_CONTAINER_GLOBALS_H */
