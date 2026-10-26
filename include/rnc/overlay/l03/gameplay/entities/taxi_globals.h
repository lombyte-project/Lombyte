#ifndef LOMBYTE_RNC_OVERLAY_L03_GAMEPLAY_ENTITIES_TAXI_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L03_GAMEPLAY_ENTITIES_TAXI_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l03/gameplay/entities/taxi.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float taxi_bobbing_amplitude __asm__("D_L03_00161AF0");
extern float taxi_bobbing_phase_rate_degrees __asm__("D_L03_00161AF4");
extern float taxi_rocking_amplitude __asm__("D_L03_00161AF8");
extern float taxi_rocking_first_phase_rate_degrees __asm__("D_L03_00161AFC");
extern float taxi_rocking_second_phase_rate_degrees __asm__("D_L03_00161B00");
extern s32 mine_layer_fire_interval_frames __asm__("D_L03_00161918");
extern f32 mine_layer_patrol_animation_speed __asm__("D_L03_0016191C");
extern f32 mine_layer_initial_scale_multiplier __asm__("D_L03_00161920");
extern f32 mine_layer_projectile_lateral_spread_speed __asm__("D_L03_00161910");
extern f32 mine_layer_projectile_vertical_launch_speed __asm__("D_L03_00161914");
extern f32 mine_layer_lethal_knockback_speed_scale __asm__("D_L03_001618F8");
extern f32 mine_layer_nonlethal_knockback_speed_scale __asm__("D_L03_001618FC");
extern s32 taxi_path_point_debug_draw_enabled __asm__("D_L03_00161B04");
extern f32 rail_flyer_max_speed __asm__("D_L03_00161B08");
extern f32 rail_flyer_pitch_acceleration_degrees __asm__("D_L03_00161B10");
extern f32 rail_flyer_roll_acceleration_degrees __asm__("D_L03_00161B18");
extern f32 rail_flyer_speed_acceleration __asm__("D_L03_00161B0C");
extern f32 rail_flyer_yaw_acceleration_degrees __asm__("D_L03_00161B14");
extern float rocket_trail_offset_z_scale __asm__("D_L03_00161B28");
extern float helga_big_head_cutscene_adjustment __asm__("D_L03_0015F5A0");
extern float helga_big_head_attachment_uniform_value __asm__("D_L03_00161BB4");
extern float path_mover_distance_step __asm__("D_L03_00161BC0");

#endif /* LOMBYTE_RNC_OVERLAY_L03_GAMEPLAY_ENTITIES_TAXI_GLOBALS_H */
