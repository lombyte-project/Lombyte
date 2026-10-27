#ifndef LOMBYTE_RNC_OVERLAY_L17_GAMEPLAY_ENTITIES_DOOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L17_GAMEPLAY_ENTITIES_DOOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l17/gameplay/entities/door.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float fighter_jet_boarding_distance_limit __asm__("D_L17_00161FE0");
extern float fighter_jet_initial_flight_speed __asm__("D_L17_00161FE4");
extern float fighter_jet_starting_hp __asm__("D_L17_00162058");
extern float fighter_jet_max_hp __asm__("D_L17_00162084");
extern float ripple_pair_radial_phase_distance_scale __asm__("D_L17_00162254");
extern float ripple_pair_phase_period __asm__("D_L17_00162258");
extern float ripple_pair_z_amplitude __asm__("D_L17_0016225C");
extern float ripple_pair_uv_scroll_rate __asm__("D_L17_00162260");
extern float ripple_uv_scroll_rate __asm__("D_L17_0016238C");
extern float ripple_radial_phase_distance_scale __asm__("D_L17_00162380");
extern float ripple_phase_period __asm__("D_L17_00162384");
extern float ripple_z_amplitude __asm__("D_L17_00162388");

/* Additional scalar data read by this unit. */
extern float fighter_jet_startup_speed_step_gp __asm__("D_L17_00161FF8") __attribute__((sda));
extern float fighter_jet_cruise_speed_step_gp __asm__("D_L17_00161FFC") __attribute__((sda));
extern float fighter_jet_cruise_speed_gp __asm__("D_L17_00162004") __attribute__((sda));
extern float fighter_jet_boost_speed_limit_gp __asm__("D_L17_00162008") __attribute__((sda));
extern float fighter_jet_boost_speed_step_gp __asm__("D_L17_0016200C") __attribute__((sda));
extern float fighter_jet_cruise_speed_return_factor_gp __asm__("D_L17_00162010") __attribute__((sda));
extern float fighter_jet_impact_yaw_response_scale_gp __asm__("D_L17_00162018") __attribute__((sda));
extern float fighter_jet_impact_pitch_response_scale_gp __asm__("D_L17_0016201C") __attribute__((sda));
extern float fighter_jet_camera_base_offset_x_gp __asm__("D_L17_00162034") __attribute__((sda));
extern float fighter_jet_camera_base_offset_z_gp __asm__("D_L17_00162038") __attribute__((sda));
extern float fighter_jet_camera_secondary_offset_x_gp __asm__("D_L17_0016203C") __attribute__((sda));
extern float fighter_jet_camera_secondary_offset_z_gp __asm__("D_L17_00162040") __attribute__((sda));
extern float fighter_jet_camera_pitch_bias_gp __asm__("D_L17_0016204C") __attribute__((sda));
extern float fighter_jet_collision_damage_gp __asm__("D_L17_00162060") __attribute__((sda));
extern float fighter_jet_horizontal_steering_error_gain_gp __asm__("D_L17_001620AC") __attribute__((sda));
extern float fighter_jet_horizontal_steering_velocity_damping_gp __asm__("D_L17_001620B0") __attribute__((sda));
extern float fighter_jet_horizontal_steering_settle_threshold_scale_gp __asm__("D_L17_001620B4") __attribute__((sda));
extern float fighter_jet_rescaled_horizontal_steering_settle_threshold_scale_gp __asm__("D_L17_001620B8") __attribute__((sda));
extern float fighter_jet_vertical_steering_error_gain_gp __asm__("D_L17_001620BC") __attribute__((sda));
extern float fighter_jet_vertical_steering_velocity_damping_gp __asm__("D_L17_001620C0") __attribute__((sda));
extern float fighter_jet_vertical_steering_settle_threshold_scale_gp __asm__("D_L17_001620C4") __attribute__((sda));
extern float fighter_jet_rescaled_vertical_steering_settle_threshold_scale_gp __asm__("D_L17_001620C8") __attribute__((sda));
extern float fighter_jet_roll_error_gain_gp __asm__("D_L17_001620CC") __attribute__((sda));
extern float fighter_jet_roll_velocity_damping_gp __asm__("D_L17_001620D0") __attribute__((sda));
extern float fighter_jet_roll_settle_threshold_scale_gp __asm__("D_L17_001620D4") __attribute__((sda));
extern float fighter_jet_camera_roll_error_gain_gp __asm__("D_L17_001620F0") __attribute__((sda));
extern float fighter_jet_camera_roll_velocity_damping_gp __asm__("D_L17_001620F4") __attribute__((sda));
extern float fighter_jet_camera_roll_settle_threshold_scale_gp __asm__("D_L17_001620F8") __attribute__((sda));
extern float fighter_jet_camera_pitch_error_gain_gp __asm__("D_L17_001620FC") __attribute__((sda));
extern float fighter_jet_camera_pitch_velocity_damping_gp __asm__("D_L17_00162100") __attribute__((sda));
extern float fighter_jet_camera_pitch_settle_threshold_scale_gp __asm__("D_L17_00162104") __attribute__((sda));
extern float fighter_jet_camera_yaw_error_gain_gp __asm__("D_L17_00162108") __attribute__((sda));
extern float fighter_jet_camera_yaw_velocity_damping_gp __asm__("D_L17_0016210C") __attribute__((sda));
extern float fighter_jet_camera_yaw_settle_threshold_scale_gp __asm__("D_L17_00162110") __attribute__((sda));
extern float fighter_jet_camera_offset_x_error_gain_gp __asm__("D_L17_00162114") __attribute__((sda));
extern float fighter_jet_camera_offset_x_velocity_damping_gp __asm__("D_L17_00162118") __attribute__((sda));
extern float fighter_jet_camera_offset_x_settle_threshold_scale_gp __asm__("D_L17_0016211C") __attribute__((sda));
extern float fighter_jet_camera_offset_y_error_gain_gp __asm__("D_L17_00162120") __attribute__((sda));
extern float fighter_jet_camera_offset_y_velocity_damping_gp __asm__("D_L17_00162124") __attribute__((sda));
extern float fighter_jet_camera_offset_y_settle_threshold_scale_gp __asm__("D_L17_00162128") __attribute__((sda));
extern float fighter_jet_camera_offset_z_error_gain_gp __asm__("D_L17_0016212C") __attribute__((sda));
extern float fighter_jet_camera_offset_z_velocity_damping_gp __asm__("D_L17_00162130") __attribute__((sda));
extern float fighter_jet_camera_offset_z_settle_threshold_scale_gp __asm__("D_L17_00162134") __attribute__((sda));
extern float fighter_jet_cruise_fov_gp __asm__("D_L17_00162144") __attribute__((sda));
extern float fighter_jet_boost_fov_gp __asm__("D_L17_00162148") __attribute__((sda));
extern float fighter_jet_fov_error_gain_gp __asm__("D_L17_0016214C") __attribute__((sda));
extern float fighter_jet_fov_velocity_damping_gp __asm__("D_L17_00162150") __attribute__((sda));
extern float fighter_jet_fov_settle_threshold_scale_gp __asm__("D_L17_00162154") __attribute__((sda));
extern float fighter_jet_transition_fade_value __asm__("D_L17_0015F3FC");
extern float ring_ripple_uv_scroll_rate_gp __asm__("D_L17_001622CC") __attribute__((sda));
extern float ring_ripple_radial_phase_distance_scale_gp __asm__("D_L17_001622C0") __attribute__((sda));
extern float ring_ripple_phase_period_gp __asm__("D_L17_001622C4") __attribute__((sda));
extern float ring_ripple_z_amplitude_gp __asm__("D_L17_001622C8") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L17_GAMEPLAY_ENTITIES_DOOR_GLOBALS_H */
