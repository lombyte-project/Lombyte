#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_DREK_BOSS_ELECTRIC_BALL_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_DREK_BOSS_ELECTRIC_BALL_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l18/mobys/drek_boss_electric_ball.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float electric_arc_endpoint_radius_scale __asm__("D_L18_00161ED8");
extern float target_travel_speed __asm__("D_L18_00161EE0");
extern int   explosion_burst_spark_count __asm__("D_L18_00161F6C");
extern int   explosion_burst_spark_frame_count __asm__("D_L18_00161F80");
extern int   explosion_burst_spark_second_frame_count __asm__("D_L18_00161F84");
extern int   explosion_burst_spark_third_frame_count __asm__("D_L18_00161F88");
extern int   explosion_burst_particle_count __asm__("D_L18_00161F98");
extern int   explosion_burst_camera_shake_duration_frames __asm__("D_L18_00161FE8");
extern float explosion_burst_spark_speed_min __asm__("D_L18_00161F70");
extern float explosion_burst_spark_speed_max __asm__("D_L18_00161F74");
extern float explosion_burst_spark_second_vector_scale __asm__("D_L18_00161F94");
extern float explosion_burst_particle_speed_min __asm__("D_L18_00161FA0");
extern float explosion_burst_particle_speed_max __asm__("D_L18_00161FA4");
extern float explosion_burst_particle_xy_offset_limit __asm__("D_L18_00161FBC");
extern float explosion_burst_camera_shake_amplitude __asm__("D_L18_00161FE4");
extern int   electric_bolt_initialization_requested __asm__("D_L18_00161FEC");
extern float primary_bolt_z_phase_step __asm__("D_L18_00161FFC");
extern float primary_bolt_z_amplitude_start __asm__("D_L18_00162000");
extern float primary_bolt_z_amplitude_end __asm__("D_L18_00162004");
extern float primary_bolt_z_phase_increment __asm__("D_L18_0016200C");
extern float secondary_bolt_orbit_phase_decrement __asm__("D_L18_00162014");
extern float secondary_bolt_orbit_phase_step __asm__("D_L18_00162018");
extern float secondary_bolt_z_amplitude __asm__("D_L18_0016201C");
extern float secondary_bolt_lateral_amplitude __asm__("D_L18_00162020");
extern float secondary_bolt_z_phase_increment __asm__("D_L18_00162028");
extern float secondary_bolt_z_phase_step __asm__("D_L18_0016202C");
extern float primary_bolt_random_step_blend_weight __asm__("D_L18_0016208C");

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_DREK_BOSS_ELECTRIC_BALL_GLOBALS_H */
