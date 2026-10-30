#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_SANDMOUSE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_SANDMOUSE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/enemies/sandmouse.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern f32   moby_quad_c0_offset_scale __asm__("D_L01_00162064");
extern f32   moby_quad_e0_offset_scale __asm__("D_L01_0016206C");
extern float sandmouse_projectile_target_range __asm__("D_L01_00162084");
extern float sandmouse_projectile_target_yaw_limit_degrees __asm__("D_L01_00162088");
extern float sandmouse_projectile_target_pitch_limit_degrees __asm__("D_L01_0016208C");
extern float sandmouse_projectile_spawn_offset_distance __asm__("D_L01_00162090");
extern float sandmouse_projectile_fallback_aim_height __asm__("D_L01_00162094");
extern f32   sandmouse_target_yaw_spring_strength_degrees __asm__("D_L01_001620C4");
extern f32   sandmouse_target_yaw_damping_degrees __asm__("D_L01_001620C8");
extern f32   sandmouse_target_yaw_turn_limit_degrees __asm__("D_L01_001620CC");
extern f32   sandmouse_hover_velocity_to_rotation_y_scale __asm__("D_L01_001620D4");
extern f32   sandmouse_hover_rotation_x_blend_factor __asm__("D_L01_001620D8");
extern f32   sandmouse_hover_rotation_y_blend_factor __asm__("D_L01_001620DC");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_SANDMOUSE_GLOBALS_H */
