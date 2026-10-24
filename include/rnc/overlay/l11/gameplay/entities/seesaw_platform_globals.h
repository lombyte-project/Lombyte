#ifndef LOMBYTE_RNC_OVERLAY_L11_GAMEPLAY_ENTITIES_SEESAW_PLATFORM_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L11_GAMEPLAY_ENTITIES_SEESAW_PLATFORM_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l11/gameplay/entities/seesaw_platform.c. */
extern float child_set_base_z_offset __asm__("D_L11_00161EAC") __attribute__((sda));
extern float child_set_base_rotation_y_degrees __asm__("D_L11_00161EBC") __attribute__((sda));
extern float final_child_set_z_offset __asm__("D_L11_00161F08") __attribute__((sda));
extern float child_grid_pose_blend_acceleration __asm__("D_L11_00161F58") __attribute__((sda));
extern float child_grid_pose_blend_deceleration __asm__("D_L11_00161F5C") __attribute__((sda));
extern float child_grid_pose_blend_max_speed __asm__("D_L11_00161F60") __attribute__((sda));
extern float child_grid_parent_axis0_offset_length __asm__("D_L11_00161F40") __attribute__((sda));
extern float child_grid_parent_axis1_offset_length __asm__("D_L11_00161F44") __attribute__((sda));
extern float child_grid_child_axis1_offset_length __asm__("D_L11_00161F48") __attribute__((sda));
extern float seesaw_hero_local_x_scale __asm__("D_L11_00161F68") __attribute__((sda));
extern float seesaw_tilt_feedback_scale __asm__("D_L11_00161F6C") __attribute__((sda));
extern float seesaw_spin_feedback_scale __asm__("D_L11_00161F70") __attribute__((sda));
extern float seesaw_rider_spin_change_bias_degrees __asm__("D_L11_00161F74") __attribute__((sda));
extern float seesaw_rider_spin_change_gain_degrees __asm__("D_L11_00161F78") __attribute__((sda));
extern float seesaw_tilt_feedback_gain_degrees __asm__("D_L11_00161F7C") __attribute__((sda));
extern float seesaw_spin_feedback_gain_degrees __asm__("D_L11_00161F80") __attribute__((sda));
extern float seesaw_fall_tilt_threshold_degrees __asm__("D_L11_00161F84") __attribute__((sda));
extern float psytcopus_projectile_forward_offset_length __asm__("D_L11_00161FC8") __attribute__((sda));
extern float psytcopus_projectile_z_offset __asm__("D_L11_00161FCC") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L11_GAMEPLAY_ENTITIES_SEESAW_PLATFORM_GLOBALS_H */
