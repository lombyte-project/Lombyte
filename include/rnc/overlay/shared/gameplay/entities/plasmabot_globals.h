#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_PLASMABOT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_PLASMABOT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/plasmabot.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float plasmabot_planar_velocity_base_scale __asm__("D_L14_001614DC");
extern float linked_moby_initial_offset_length __asm__("D_L14_00161FC4");
extern float linked_moby_additional_offset_length __asm__("D_L14_00161FC8");
extern int plasmabot_path_traversal_delay_frames __asm__("D_L14_001614F0");
extern f32 plasmabot_base_target_range __asm__("D_L14_001614C4");
extern float plasmabot_stagger_motion_z_scale __asm__("D_L14_001614C8");
extern float plasmabot_path_traversal_speed __asm__("D_L14_001614D8");
extern float plasmabot_glow_quad_scale __asm__("D_L14_001614F8");
extern float plasmabot_glow_camera_pull_distance_limit __asm__("D_L14_001614FC");
extern float plasmabot_glow_camera_clearance_distance __asm__("D_L14_00161500");
extern u32 plasmabot_glow_vertex_color __asm__("D_L14_00161504");
extern float plasmabot_debris_launch_magnitude_range_start __asm__("D_L14_00161B08");
extern float plasmabot_debris_launch_magnitude_range_end __asm__("D_L14_00161B0C");
extern float plasmabot_debris_z_velocity_range_start __asm__("D_L14_00161B10");
extern float plasmabot_debris_z_velocity_range_end __asm__("D_L14_00161B14");
extern f32 ballistic_debris_gravity __asm__("D_L14_00161B40");
extern f32 ballistic_debris_velocity_base_scale __asm__("D_L14_00161B44");
extern f32 ballistic_debris_planar_damping_speed_threshold __asm__("D_L14_00161B48");
extern float ballistic_debris_launch_magnitude_range_start __asm__("D_L14_00161B30");
extern float ballistic_debris_launch_magnitude_range_end __asm__("D_L14_00161B34");
extern float ballistic_debris_z_velocity_range_start __asm__("D_L14_00161B38");
extern float ballistic_debris_z_velocity_range_end __asm__("D_L14_00161B3C");
extern float particle_pair_moby_c0_offset_scale __asm__("D_L14_00161FCC");
extern float particle_pair_moby_e0_offset_scale __asm__("D_L14_00161FD0");
extern float particle_trio_moby_c0_offset_scale __asm__("D_L14_00161FD4");
extern float particle_trio_moby_e0_offset_scale __asm__("D_L14_00161FD8");
extern int particle_pair_color __asm__("D_L14_00161FE4");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_PLASMABOT_GLOBALS_H */
