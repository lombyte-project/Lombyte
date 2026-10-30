#ifndef LOMBYTE_RNC_OVERLAY_L14_MOBYS_MORPHORAY_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L14_MOBYS_MORPHORAY_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l14/mobys/morphoray.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float spark_contact_clearance __asm__("D_L14_00162088");
extern float spark_minimum_speed __asm__("D_L14_00162090");
extern float spark_rolling_speed_factor __asm__("D_L14_00162094");
extern float spark_initial_moby_value_multiplier __asm__("D_L14_00162098");
extern f32   morphoray_ground_height_offset __asm__("D_L14_001621E8");
extern f32   morphoray_yaw_step __asm__("D_L14_001621EC");
extern f32   morphoray_position_offset_scale __asm__("D_L14_001621F0");
extern int   flying_car_path_reset_delay_frames __asm__("D_L14_0016227C");
extern s32   path_camera_active __asm__("D_L14_0015F404");

/* Additional scalar data read by this unit. */
extern float spark_first_bounce_horizontal_speed_gp __asm__("D_L14_001620A4") __attribute__((sda));
extern float flying_car_rotation_oscillation_amplitude_gp __asm__("D_L14_00162290")
    __attribute__((sda));
extern float flying_car_rotation_oscillation_common_phase_update_gp __asm__("D_L14_00162294")
    __attribute__((sda));
extern float flying_car_rotation_oscillation_axis_phase_update_gp __asm__("D_L14_00162298")
    __attribute__((sda));
extern float flying_car_tracking_spring_gain_gp __asm__("D_L14_0016226C") __attribute__((sda));
extern float flying_car_motion_spring_damping_gp __asm__("D_L14_00162270") __attribute__((sda));
extern float flying_car_full_bank_yaw_error_gp __asm__("D_L14_00162278") __attribute__((sda));
extern float flying_car_max_sink_offset_rate_gp __asm__("D_L14_00162280") __attribute__((sda));
extern float flying_car_sink_offset_recovery_rate_gp __asm__("D_L14_00162288") __attribute__((sda));
extern float flying_car_burst_height_threshold_gp __asm__("D_L14_0016228C") __attribute__((sda));
extern float path_key_camera_heading_offset_spring_gain __asm__("D_L14_00162490")
    __attribute__((sda));
extern float path_key_camera_placement_elevation_spring_gain __asm__("D_L14_00162494")
    __attribute__((sda));
extern float path_key_camera_placement_distance_spring_gain __asm__("D_L14_00162498")
    __attribute__((sda));
extern float path_key_camera_lateral_axis_rotation_spring_gain __asm__("D_L14_0016249C")
    __attribute__((sda));
extern float path_key_camera_up_axis_rotation_spring_gain __asm__("D_L14_001624A0")
    __attribute__((sda));
extern float path_key_camera_heading_blend_step_factor __asm__("D_L14_001624A4")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L14_MOBYS_MORPHORAY_GLOBALS_H */
