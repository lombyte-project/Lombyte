#ifndef LOMBYTE_RNC_OVERLAY_L01_GAMEPLAY_ENTITIES_BIRDBOT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L01_GAMEPLAY_ENTITIES_BIRDBOT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l01/gameplay/entities/birdbot.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern f32 water_lily_spring_velocity_adjustment_scale __asm__("D_L01_00161BF0");
extern f32 water_lily_spring_damping __asm__("D_L01_00161BF4");
extern f32 water_lily_hero_separation_radius __asm__("D_L01_00161BF8");
extern f32 water_lily_max_horizontal_speed __asm__("D_L01_00161BFC");
extern f32 water_lily_collision_radius __asm__("D_L01_00161C00");
extern f32 falling_rock_camera_rotation_angle __asm__("D_L01_00161C28");
extern f32 falling_rock_camera_distance __asm__("D_L01_00161C2C");
extern f32 falling_rock_camera_height __asm__("D_L01_00161C30");
extern f32 falling_rock_camera_offset_scale __asm__("D_L01_00161C34");
extern f32 falling_rock_camera_override_companion_value __asm__("D_L01_00161C38");

/* Additional scalar data read by this unit. */
extern f32 peckbot_nonfatal_knockback_vertical_speed_gp __asm__("D_L01_00161A60") __attribute__((sda));
extern f32 peckbot_nonfatal_knockback_horizontal_speed_gp __asm__("D_L01_00161A64") __attribute__((sda));
extern f32 peckbot_home_knockback_speed_gp __asm__("D_L01_00161A68") __attribute__((sda));
extern f32 peckbot_home_knockback_acceleration_gp __asm__("D_L01_00161A6C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L01_GAMEPLAY_ENTITIES_BIRDBOT_GLOBALS_H */
