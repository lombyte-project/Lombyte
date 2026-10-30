#ifndef LOMBYTE_RNC_OVERLAY_L16_MOBYS_ROCKET_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L16_MOBYS_ROCKET_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l16/mobys/rocket.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float test_dummy_door_height_offset __asm__("D_L16_00161A84");
extern int   last_test_dummy_door_sound_request_frame __asm__("D_L16_00161A88");
extern float reposition_effect_velocity_elevation_degrees __asm__("D_L16_001618E8");
extern float reposition_effect_velocity_scale_range_start __asm__("D_L16_001618EC");
extern float reposition_effect_velocity_scale_range_end __asm__("D_L16_001618F0");
extern float reposition_effect_spawn_z_offset __asm__("D_L16_001618F4");
extern float reposition_effect_radius_range_start __asm__("D_L16_001618F8");
extern float reposition_effect_radius_range_end __asm__("D_L16_001618FC");
extern float conveyor_belt_base_speed __asm__("D_L16_0016198C");
extern int   conveyor_belt_pulse_texture_id __asm__("D_L16_00161990");
extern int   conveyor_belt_scrolling_texture_id __asm__("D_L16_00161994");
extern int   conveyor_belt_pulse_color_start __asm__("D_L16_001619A8");
extern int   conveyor_belt_pulse_color_end __asm__("D_L16_001619AC");
extern int   conveyor_belt_scrolling_vertex_color __asm__("D_L16_001619B0");
extern float test_dummy_bomb_horizontal_launch_speed __asm__("D_L16_001619CC");
extern float test_dummy_glow_phase_rate_degrees __asm__("D_L16_001619DC");
extern int   test_dummy_glow_pulse_color_start __asm__("D_L16_001619E0");
extern int   test_dummy_glow_pulse_color_end __asm__("D_L16_001619E4");
extern int   hoverboard_dummy_spray_initial_color __asm__("D_L16_00161A08");
extern float class_0287_group_activation_value __asm__("D_L16_00161A80");
extern float triggered_scene_rotation_y __asm__("D_L16_00161A8C");

#endif /* LOMBYTE_RNC_OVERLAY_L16_MOBYS_ROCKET_GLOBALS_H */
