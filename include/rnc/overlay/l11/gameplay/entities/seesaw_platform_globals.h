#ifndef LOMBYTE_RNC_OVERLAY_L11_GAMEPLAY_ENTITIES_SEESAW_PLATFORM_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L11_GAMEPLAY_ENTITIES_SEESAW_PLATFORM_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l11/gameplay/entities/seesaw_platform.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float child_set_base_z_offset __asm__("D_L11_00161EAC");
extern float child_set_base_rotation_y_degrees __asm__("D_L11_00161EBC");
extern float final_child_set_z_offset __asm__("D_L11_00161F08");
extern float child_grid_pose_blend_acceleration __asm__("D_L11_00161F58");
extern float child_grid_pose_blend_deceleration __asm__("D_L11_00161F5C");
extern float child_grid_pose_blend_max_speed __asm__("D_L11_00161F60");
extern float child_grid_parent_axis0_offset_length __asm__("D_L11_00161F40");
extern float child_grid_parent_axis1_offset_length __asm__("D_L11_00161F44");
extern float child_grid_child_axis1_offset_length __asm__("D_L11_00161F48");
extern float seesaw_hero_local_x_scale __asm__("D_L11_00161F68");
extern float seesaw_tilt_feedback_scale __asm__("D_L11_00161F6C");
extern float seesaw_spin_feedback_scale __asm__("D_L11_00161F70");
extern float seesaw_rider_spin_change_bias_degrees __asm__("D_L11_00161F74");
extern float seesaw_rider_spin_change_gain_degrees __asm__("D_L11_00161F78");
extern float seesaw_tilt_feedback_gain_degrees __asm__("D_L11_00161F7C");
extern float seesaw_spin_feedback_gain_degrees __asm__("D_L11_00161F80");
extern float seesaw_fall_tilt_threshold_degrees __asm__("D_L11_00161F84");
extern float psytcopus_projectile_forward_offset_length __asm__("D_L11_00161FC8");
extern float psytcopus_projectile_z_offset __asm__("D_L11_00161FCC");

/* Additional scalar data read by this unit. */
extern float child_set_child_axis2_offset_length_gp __asm__("D_L11_00161EB0") __attribute__((sda));
extern float child_set_fold_rotation_x_degrees_gp __asm__("D_L11_00161EB8") __attribute__((sda));
extern float attached_child_source_axis2_offset_length_gp __asm__("D_L11_00161EFC") __attribute__((sda));
extern float attached_child_source_axis1_offset_length_gp __asm__("D_L11_00161F00") __attribute__((sda));
extern float attached_child_rotation_y_degrees_gp __asm__("D_L11_00161F04") __attribute__((sda));
extern float psytcopus_knockback_base_gravity_gp __asm__("D_L11_00161F88") __attribute__((sda));
extern float psytcopus_hurt_knockback_base_vertical_speed_gp __asm__("D_L11_00161F8C") __attribute__((sda));
extern float psytcopus_hurt_knockback_base_horizontal_speed_gp __asm__("D_L11_00161F90") __attribute__((sda));
extern float psytcopus_death_knockback_base_vertical_speed_gp __asm__("D_L11_00161F94") __attribute__((sda));
extern float psytcopus_death_knockback_base_horizontal_speed_gp __asm__("D_L11_00161F98") __attribute__((sda));
extern float jet_fighter_startup_speed_increment_gp __asm__("D_L11_00161FE8") __attribute__((sda));
extern float jet_fighter_cruise_speed_increment_gp __asm__("D_L11_00161FEC") __attribute__((sda));
extern float jet_fighter_boost_speed_limit_gp __asm__("D_L11_00161FF8") __attribute__((sda));
extern float jet_fighter_boost_speed_increment_gp __asm__("D_L11_00161FFC") __attribute__((sda));
extern float jet_fighter_cruise_speed_relaxation_factor_gp __asm__("D_L11_00162000") __attribute__((sda));
extern float jet_fighter_yaw_deflection_scale_gp __asm__("D_L11_00162008") __attribute__((sda));
extern float jet_fighter_pitch_deflection_scale_gp __asm__("D_L11_0016200C") __attribute__((sda));
extern float jet_fighter_camera_base_local_x_offset_gp __asm__("D_L11_00162024") __attribute__((sda));
extern float jet_fighter_camera_base_local_z_offset_gp __asm__("D_L11_00162028") __attribute__((sda));
extern float jet_fighter_alternate_camera_local_x_offset_gp __asm__("D_L11_0016202C") __attribute__((sda));
extern float jet_fighter_alternate_camera_local_z_offset_gp __asm__("D_L11_00162030") __attribute__((sda));
extern float jet_fighter_camera_pitch_offset_radians_gp __asm__("D_L11_0016203C") __attribute__((sda));
extern float jet_fighter_collision_damage_gp __asm__("D_L11_00162050") __attribute__((sda));
extern float jet_fighter_horizontal_steering_snap_threshold_scale_gp __asm__("D_L11_001620A4") __attribute__((sda));
extern float jet_fighter_l3_horizontal_steering_snap_threshold_scale_gp __asm__("D_L11_001620A8") __attribute__((sda));
extern float jet_fighter_vertical_steering_snap_threshold_scale_gp __asm__("D_L11_001620B4") __attribute__((sda));
extern float jet_fighter_l3_vertical_steering_snap_threshold_scale_gp __asm__("D_L11_001620B8") __attribute__((sda));
extern float jet_fighter_rotation_x_snap_threshold_scale_gp __asm__("D_L11_001620C4") __attribute__((sda));
extern float jet_fighter_camera_rotation_x_snap_threshold_scale_gp __asm__("D_L11_001620E8") __attribute__((sda));
extern float jet_fighter_camera_rotation_y_snap_threshold_scale_gp __asm__("D_L11_001620F4") __attribute__((sda));
extern float jet_fighter_camera_rotation_z_snap_threshold_scale_gp __asm__("D_L11_00162100") __attribute__((sda));
extern float jet_fighter_camera_local_x_snap_threshold_scale_gp __asm__("D_L11_0016210C") __attribute__((sda));
extern float jet_fighter_camera_local_y_snap_threshold_scale_gp __asm__("D_L11_00162118") __attribute__((sda));
extern float jet_fighter_camera_local_z_snap_threshold_scale_gp __asm__("D_L11_00162124") __attribute__((sda));
extern float jet_fighter_cruise_camera_fov_radians_gp __asm__("D_L11_00162134") __attribute__((sda));
extern float jet_fighter_boost_camera_fov_radians_gp __asm__("D_L11_00162138") __attribute__((sda));
extern float jet_fighter_boarding_distance_gp __asm__("D_L11_00161FD0") __attribute__((sda));
extern float jet_fighter_initial_flight_speed_gp __asm__("D_L11_00161FD4") __attribute__((sda));
extern float jet_fighter_initial_health_gp __asm__("D_L11_00162048") __attribute__((sda));
extern float jet_fighter_max_health_gp __asm__("D_L11_00162074") __attribute__((sda));
extern float puffoid_spray_target_distance_metric_threshold_gp __asm__("D_L11_001622E4") __attribute__((sda));
extern float puffoid_chase_heading_offset_limit_degrees_gp __asm__("D_L11_001622E8") __attribute__((sda));
extern float puffoid_jump_horizontal_speed_gp __asm__("D_L11_001622EC") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L11_GAMEPLAY_ENTITIES_SEESAW_PLATFORM_GLOBALS_H */
