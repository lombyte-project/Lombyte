#ifndef LOMBYTE_RNC_OVERLAY_L12_MOBYS_HOVEN_ANKLEBITER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L12_MOBYS_HOVEN_ANKLEBITER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l12/mobys/hoven_anklebiter.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float spark_scatter_vertical_z_range_start __asm__("D_L12_00161888");
extern float spark_scatter_vertical_z_range_end __asm__("D_L12_0016188C");
extern float spark_scatter_radial_z_range_start __asm__("D_L12_00161890");
extern float spark_scatter_radial_z_range_end __asm__("D_L12_00161894");
extern float spark_scatter_radial_xy_scale_range_start __asm__("D_L12_00161898");
extern float spark_scatter_radial_xy_scale_range_end __asm__("D_L12_0016189C");
extern float spark_scatter_vertical_vector_w __asm__("D_L12_001618A0");
extern float spark_scatter_radial_vector_w __asm__("D_L12_001618A4");
extern int   spark_scatter_frame_count_range_start __asm__("D_L12_001618AC");
extern int   spark_scatter_frame_count_range_end __asm__("D_L12_001618B0");
extern int   spark_scatter_second_frame_count_range_start __asm__("D_L12_001618B4");
extern int   spark_scatter_second_frame_count_range_end __asm__("D_L12_001618B8");
extern int   spark_scatter_third_frame_count_range_start __asm__("D_L12_001618BC");
extern int   spark_scatter_third_frame_count_range_end __asm__("D_L12_001618C0");
extern float hoven_anklebiter_leap_launch_speed __asm__("D_L12_001618D8");
extern float hoven_anklebiter_leap_gravity __asm__("D_L12_001618DC");

#endif /* LOMBYTE_RNC_OVERLAY_L12_MOBYS_HOVEN_ANKLEBITER_GLOBALS_H */
