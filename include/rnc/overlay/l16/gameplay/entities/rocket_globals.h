#ifndef LOMBYTE_RNC_OVERLAY_L16_GAMEPLAY_ENTITIES_ROCKET_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L16_GAMEPLAY_ENTITIES_ROCKET_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l16/gameplay/entities/rocket.c. */
extern float test_dummy_door_height_offset __asm__("D_L16_00161A84") __attribute__((sda));
extern int last_test_dummy_door_sound_request_frame __asm__("D_L16_00161A88") __attribute__((sda));
extern float reposition_effect_velocity_elevation_degrees __asm__("D_L16_001618E8") __attribute__((sda));
extern float reposition_effect_velocity_scale_range_start __asm__("D_L16_001618EC") __attribute__((sda));
extern float reposition_effect_velocity_scale_range_end __asm__("D_L16_001618F0") __attribute__((sda));
extern float reposition_effect_spawn_z_offset __asm__("D_L16_001618F4") __attribute__((sda));
extern float reposition_effect_radius_range_start __asm__("D_L16_001618F8") __attribute__((sda));
extern float reposition_effect_radius_range_end __asm__("D_L16_001618FC") __attribute__((sda));
extern float conveyor_belt_base_speed __asm__("D_L16_0016198C") __attribute__((sda));
extern int conveyor_belt_pulse_texture_id __asm__("D_L16_00161990") __attribute__((sda));
extern int conveyor_belt_scrolling_texture_id __asm__("D_L16_00161994") __attribute__((sda));
extern int conveyor_belt_pulse_color_start __asm__("D_L16_001619A8") __attribute__((sda));
extern int conveyor_belt_pulse_color_end __asm__("D_L16_001619AC") __attribute__((sda));
extern int conveyor_belt_scrolling_vertex_color __asm__("D_L16_001619B0") __attribute__((sda));
extern float test_dummy_bomb_horizontal_launch_speed __asm__("D_L16_001619CC") __attribute__((sda));
extern float test_dummy_glow_phase_rate_degrees __asm__("D_L16_001619DC");
extern int test_dummy_glow_pulse_color_start __asm__("D_L16_001619E0") __attribute__((sda));
extern int test_dummy_glow_pulse_color_end __asm__("D_L16_001619E4") __attribute__((sda));
extern int hoverboard_dummy_spray_initial_color __asm__("D_L16_00161A08") __attribute__((sda));
extern float class_0287_group_activation_value __asm__("D_L16_00161A80") __attribute__((sda));
extern float triggered_scene_rotation_y __asm__("D_L16_00161A8C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L16_GAMEPLAY_ENTITIES_ROCKET_GLOBALS_H */
