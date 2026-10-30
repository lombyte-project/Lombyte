#ifndef LOMBYTE_RNC_OVERLAY_L00_MOBYS_VELDIN_ROBOT_ENEMY_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L00_MOBYS_VELDIN_ROBOT_ENEMY_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l00/mobys/veldin_robot_enemy.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern f32 veldin_robot_probe_length __asm__("D_L00_00161BE4");
extern f32 veldin_robot_probe_heading_offset __asm__("D_L00_00161BEC");
extern f32 veldin_robot_default_target_range __asm__("D_L00_00161BD0");
extern f32 veldin_robot_knockback_xy_scale __asm__("D_L00_00161BDC");
extern f32 veldin_robot_knockback_z_scale __asm__("D_L00_00161BE0");

/* Additional scalar data read by this unit. */
extern f32 veldin_robot_knockback_gravity_gp __asm__("D_L00_00161BD4") __attribute__((sda));
extern f32 veldin_robot_knockback_xy_speed_step_gp __asm__("D_L00_00161BD8") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L00_MOBYS_VELDIN_ROBOT_ENEMY_GLOBALS_H */
