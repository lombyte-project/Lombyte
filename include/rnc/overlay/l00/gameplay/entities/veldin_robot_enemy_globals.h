#ifndef LOMBYTE_RNC_OVERLAY_L00_GAMEPLAY_ENTITIES_VELDIN_ROBOT_ENEMY_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L00_GAMEPLAY_ENTITIES_VELDIN_ROBOT_ENEMY_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l00/gameplay/entities/veldin_robot_enemy.c. */
extern f32 veldin_robot_probe_length __asm__("D_L00_00161BE4") __attribute__((sda));
extern f32 veldin_robot_probe_heading_offset __asm__("D_L00_00161BEC") __attribute__((sda));
extern f32 veldin_robot_default_target_range __asm__("D_L00_00161BD0") __attribute__((sda));
extern f32 veldin_robot_knockback_xy_scale __asm__("D_L00_00161BDC") __attribute__((sda));
extern f32 veldin_robot_knockback_z_scale __asm__("D_L00_00161BE0") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L00_GAMEPLAY_ENTITIES_VELDIN_ROBOT_ENEMY_GLOBALS_H */
