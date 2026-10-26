#ifndef LOMBYTE_RNC_OVERLAY_L12_GAMEPLAY_ENTITIES_PLATFORM_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L12_GAMEPLAY_ENTITIES_PLATFORM_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l12/gameplay/entities/platform.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int next_water_tile_index __asm__("D_L12_00151388");
extern float spark_scatter_vertical_z_range_start __asm__("D_L12_00161888");
extern float spark_scatter_vertical_z_range_end __asm__("D_L12_0016188C");
extern float spark_scatter_radial_z_range_start __asm__("D_L12_00161890");
extern float spark_scatter_radial_z_range_end __asm__("D_L12_00161894");
extern float spark_scatter_radial_xy_scale_range_start __asm__("D_L12_00161898");
extern float spark_scatter_radial_xy_scale_range_end __asm__("D_L12_0016189C");
extern float spark_scatter_vertical_vector_w __asm__("D_L12_001618A0");
extern float spark_scatter_radial_vector_w __asm__("D_L12_001618A4");
extern int spark_scatter_frame_count_range_start __asm__("D_L12_001618AC");
extern int spark_scatter_frame_count_range_end __asm__("D_L12_001618B0");
extern int spark_scatter_second_frame_count_range_start __asm__("D_L12_001618B4");
extern int spark_scatter_second_frame_count_range_end __asm__("D_L12_001618B8");
extern int spark_scatter_third_frame_count_range_start __asm__("D_L12_001618BC");
extern int spark_scatter_third_frame_count_range_end __asm__("D_L12_001618C0");
extern float hoven_anklebiter_leap_launch_speed __asm__("D_L12_001618D8");
extern float hoven_anklebiter_leap_gravity __asm__("D_L12_001618DC");
extern float platform_tilt_approach_divisor __asm__("D_L12_00161918");
extern float platform_tilt_offset_divisor __asm__("D_L12_00161914");
extern int helicopter_help_message_hit_count __asm__("D_0015EE04");
extern float helicopter_path_speed __asm__("D_L12_001619A8");
extern float helicopter_rotation_smoothing_divisor __asm__("D_L12_001619B0");

#endif /* LOMBYTE_RNC_OVERLAY_L12_GAMEPLAY_ENTITIES_PLATFORM_GLOBALS_H */
