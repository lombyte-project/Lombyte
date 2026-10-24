#ifndef LOMBYTE_RNC_OVERLAY_L01_GAMEPLAY_ENTITIES_BIRDBOT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L01_GAMEPLAY_ENTITIES_BIRDBOT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l01/gameplay/entities/birdbot.c. */
extern f32 water_lily_spring_velocity_adjustment_scale __asm__("D_L01_00161BF0") __attribute__((sda));
extern f32 water_lily_spring_damping __asm__("D_L01_00161BF4") __attribute__((sda));
extern f32 water_lily_hero_separation_radius __asm__("D_L01_00161BF8") __attribute__((sda));
extern f32 water_lily_max_horizontal_speed __asm__("D_L01_00161BFC") __attribute__((sda));
extern f32 water_lily_collision_radius __asm__("D_L01_00161C00") __attribute__((sda));
extern f32 falling_rock_camera_rotation_angle __asm__("D_L01_00161C28") __attribute__((sda));
extern f32 falling_rock_camera_distance __asm__("D_L01_00161C2C") __attribute__((sda));
extern f32 falling_rock_camera_height __asm__("D_L01_00161C30") __attribute__((sda));
extern f32 falling_rock_camera_offset_scale __asm__("D_L01_00161C34") __attribute__((sda));
extern f32 falling_rock_camera_override_companion_value __asm__("D_L01_00161C38") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L01_GAMEPLAY_ENTITIES_BIRDBOT_GLOBALS_H */
