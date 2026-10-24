#ifndef LOMBYTE_RNC_OVERLAY_L12_GAMEPLAY_ENTITIES_PLATFORM_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L12_GAMEPLAY_ENTITIES_PLATFORM_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l12/gameplay/entities/platform.c. */
extern int next_water_tile_index __asm__("D_L12_00151388") __attribute__((sda));
extern float spark_scatter_vertical_z_range_start __asm__("D_L12_00161888") __attribute__((sda));
extern float spark_scatter_vertical_z_range_end __asm__("D_L12_0016188C") __attribute__((sda));
extern float spark_scatter_radial_z_range_start __asm__("D_L12_00161890") __attribute__((sda));
extern float spark_scatter_radial_z_range_end __asm__("D_L12_00161894") __attribute__((sda));
extern float spark_scatter_radial_xy_scale_range_start __asm__("D_L12_00161898") __attribute__((sda));
extern float spark_scatter_radial_xy_scale_range_end __asm__("D_L12_0016189C") __attribute__((sda));
extern float spark_scatter_vertical_vector_w __asm__("D_L12_001618A0") __attribute__((sda));
extern float spark_scatter_radial_vector_w __asm__("D_L12_001618A4") __attribute__((sda));
extern int spark_scatter_frame_count_range_start __asm__("D_L12_001618AC") __attribute__((sda));
extern int spark_scatter_frame_count_range_end __asm__("D_L12_001618B0") __attribute__((sda));
extern int spark_scatter_second_frame_count_range_start __asm__("D_L12_001618B4") __attribute__((sda));
extern int spark_scatter_second_frame_count_range_end __asm__("D_L12_001618B8") __attribute__((sda));
extern int spark_scatter_third_frame_count_range_start __asm__("D_L12_001618BC") __attribute__((sda));
extern int spark_scatter_third_frame_count_range_end __asm__("D_L12_001618C0") __attribute__((sda));
extern float hoven_anklebiter_leap_launch_speed __asm__("D_L12_001618D8") __attribute__((sda));
extern float hoven_anklebiter_leap_gravity __asm__("D_L12_001618DC") __attribute__((sda));
extern float platform_tilt_approach_divisor __asm__("D_L12_00161918") __attribute__((sda));
extern float platform_tilt_offset_divisor __asm__("D_L12_00161914") __attribute__((sda));
extern int helicopter_help_message_hit_count __asm__("D_0015EE04") __attribute__((sda));
extern float helicopter_path_speed __asm__("D_L12_001619A8") __attribute__((sda));
extern float helicopter_rotation_smoothing_divisor __asm__("D_L12_001619B0") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L12_GAMEPLAY_ENTITIES_PLATFORM_GLOBALS_H */
