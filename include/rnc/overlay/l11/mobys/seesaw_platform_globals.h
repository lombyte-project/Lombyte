#ifndef LOMBYTE_RNC_OVERLAY_L11_MOBYS_SEESAW_PLATFORM_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L11_MOBYS_SEESAW_PLATFORM_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l11/mobys/seesaw_platform.c. */
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

#endif /* LOMBYTE_RNC_OVERLAY_L11_MOBYS_SEESAW_PLATFORM_GLOBALS_H */
