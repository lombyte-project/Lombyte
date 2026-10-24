#ifndef LOMBYTE_RNC_OVERLAY_L02_GAMEPLAY_ENTITIES_SAND_SHARK_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L02_GAMEPLAY_ENTITIES_SAND_SHARK_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l02/gameplay/entities/sand_shark.c. */
extern float sand_shark_dust_vertical_velocity_scale __asm__("D_L02_001619D4") __attribute__((sda));
extern float sand_shark_dust_position_jitter_radius __asm__("D_L02_001619DC") __attribute__((sda));
extern float sand_shark_vertical_motion_z_range_start __asm__("D_L02_00161920") __attribute__((sda));
extern float sand_shark_vertical_motion_z_range_end __asm__("D_L02_00161924") __attribute__((sda));
extern float sand_shark_radial_motion_z_range_start __asm__("D_L02_00161928") __attribute__((sda));
extern float sand_shark_radial_motion_z_range_end __asm__("D_L02_0016192C") __attribute__((sda));
extern float sand_shark_radial_motion_xy_range_start __asm__("D_L02_00161930") __attribute__((sda));
extern float sand_shark_radial_motion_xy_range_end __asm__("D_L02_00161934") __attribute__((sda));
extern float sand_shark_vertical_motion_w __asm__("D_L02_00161938") __attribute__((sda));
extern float sand_shark_radial_motion_w __asm__("D_L02_0016193C") __attribute__((sda));
extern int sand_shark_particle_frame_count_range_start __asm__("D_L02_00161940") __attribute__((sda));
extern int sand_shark_particle_frame_count_range_end __asm__("D_L02_00161944") __attribute__((sda));
extern int sand_shark_particle_second_frame_count_range_start __asm__("D_L02_00161948") __attribute__((sda));
extern int sand_shark_particle_second_frame_count_range_end __asm__("D_L02_0016194C") __attribute__((sda));
extern int sand_shark_particle_third_frame_count_range_start __asm__("D_L02_00161950") __attribute__((sda));
extern int sand_shark_particle_third_frame_count_range_end __asm__("D_L02_00161954") __attribute__((sda));
extern float sand_shark_single_particle_vertical_motion_z_range_start __asm__("D_L02_00161964") __attribute__((sda));
extern float sand_shark_single_particle_vertical_motion_z_range_end __asm__("D_L02_00161968") __attribute__((sda));
extern float sand_shark_single_particle_radial_motion_z_range_start __asm__("D_L02_0016196C") __attribute__((sda));
extern float sand_shark_single_particle_radial_motion_z_range_end __asm__("D_L02_00161970") __attribute__((sda));
extern float sand_shark_single_particle_radial_motion_xy_range_start __asm__("D_L02_00161974") __attribute__((sda));
extern float sand_shark_single_particle_radial_motion_xy_range_end __asm__("D_L02_00161978") __attribute__((sda));
extern float sand_shark_single_particle_vertical_motion_w __asm__("D_L02_0016197C") __attribute__((sda));
extern float sand_shark_single_particle_radial_motion_w __asm__("D_L02_00161980") __attribute__((sda));
extern s32 sand_shark_single_particle_frame_count_range_start __asm__("D_L02_00161984") __attribute__((sda));
extern s32 sand_shark_single_particle_frame_count_range_end __asm__("D_L02_00161988") __attribute__((sda));
extern s32 sand_shark_single_particle_second_frame_count_range_start __asm__("D_L02_0016198C") __attribute__((sda));
extern s32 sand_shark_single_particle_second_frame_count_range_end __asm__("D_L02_00161990") __attribute__((sda));
extern s32 sand_shark_single_particle_third_frame_count_range_start __asm__("D_L02_00161994") __attribute__((sda));
extern s32 sand_shark_single_particle_third_frame_count_range_end __asm__("D_L02_00161998") __attribute__((sda));
extern float level02_bot_wake_spray_planar_magnitude_max __asm__("D_L02_00161A98") __attribute__((sda));
extern float level02_bot_death_knockback_horizontal_speed __asm__("D_L02_00161A64") __attribute__((sda));
extern float level02_bot_death_knockback_vertical_speed __asm__("D_L02_00161A68") __attribute__((sda));
extern float level02_bot_hit_knockback_horizontal_speed __asm__("D_L02_00161A6C") __attribute__((sda));
extern float level02_bot_hit_knockback_vertical_speed __asm__("D_L02_00161A70") __attribute__((sda));
extern float level02_elevator_stop_height_offset __asm__("D_L02_00161BD0") __attribute__((sda));
extern float level02_elevator_max_vertical_speed __asm__("D_L02_00161BDC") __attribute__((sda));
extern float level02_elevator_vertical_acceleration __asm__("D_L02_00161BE0") __attribute__((sda));
extern float level02_elevator_ring_outer_radius __asm__("D_L02_00161BEC") __attribute__((sda));
extern float level02_elevator_ring_vertical_spacing __asm__("D_L02_00161BE8") __attribute__((sda));
extern float level02_elevator_ring_segment_half_height __asm__("D_L02_00161BF0") __attribute__((sda));
extern float level02_elevator_ring_segment_angle_degrees __asm__("D_L02_00161C10") __attribute__((sda));
extern f32 rocket_launcher_projectile_speed __asm__("D_L02_00161C6C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L02_GAMEPLAY_ENTITIES_SAND_SHARK_GLOBALS_H */
