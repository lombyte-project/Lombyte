#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_VELDIN_SHIP_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_VELDIN_SHIP_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/veldin_ship.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int metal_detector_trail_initial_quad_color __asm__("D_L00_00161914");
extern int metal_detector_trail_texture_selector __asm__("D_L00_00161918");
extern float horny_toad_target_approach_distance_threshold __asm__("D_L00_00161980");
extern float horny_toad_target_animation_start_index __asm__("D_L00_00161984");
extern int smoke_fire_emission_attempts_per_update __asm__("D_L00_001619BC");
extern int smoke_fire_particle_steady_alpha __asm__("D_L00_00161998");

/* Additional scalar data read by this unit. */
extern f32 crate_drop_horizontal_launch_speed_max_gp __asm__("D_L00_001618C8") __attribute__((sda));
extern f32 crate_drop_vertical_launch_speed_min_gp __asm__("D_L00_001618CC") __attribute__((sda));
extern f32 crate_drop_vertical_launch_speed_max_gp __asm__("D_L00_001618D0") __attribute__((sda));
extern int metal_detector_trail_blend_minuend_selector_gp __asm__("D_L00_00161900") __attribute__((sda));
extern int metal_detector_trail_blend_subtrahend_selector_gp __asm__("D_L00_00161904") __attribute__((sda));
extern int metal_detector_trail_blend_alpha_factor_selector_gp __asm__("D_L00_00161908") __attribute__((sda));
extern int metal_detector_trail_blend_addend_selector_gp __asm__("D_L00_0016190C") __attribute__((sda));
extern int metal_detector_trail_blend_fixed_alpha_gp __asm__("D_L00_00161910") __attribute__((sda));
extern f32 metal_detector_aim_yaw_limit_degrees_gp __asm__("D_L00_00161950") __attribute__((sda));
extern f32 metal_detector_aim_pitch_limit_degrees_gp __asm__("D_L00_00161954") __attribute__((sda));
extern s32 metal_detector_aim_perpendicular_cross_direction_row_index_gp __asm__("D_L00_0016191C") __attribute__((sda));
extern s32 metal_detector_aim_direction_row_index_gp __asm__("D_L00_00161920") __attribute__((sda));
extern s32 metal_detector_aim_perpendicular_row_index_gp __asm__("D_L00_00161924") __attribute__((sda));
extern s32 metal_detector_aim_direction_normalization_length_gp __asm__("D_L00_0016192C") __attribute__((sda));
extern s32 metal_detector_aim_perpendicular_normalization_length_gp __asm__("D_L00_00161930") __attribute__((sda));
extern f32 metal_detector_aim_cross_reference_x_gp __asm__("D_L00_00161940") __attribute__((sda));
extern float infobot_slot_path_minimum_speed_gp __asm__("D_L00_00161990") __attribute__((sda));
extern float smoke_fire_emitted_particle_position_spread_gp __asm__("D_L00_001619B4") __attribute__((sda));
extern float smoke_fire_emitted_particle_random_scalar_upper_bound_gp __asm__("D_L00_001619B8") __attribute__((sda));
extern int smoke_fire_emitted_particle_sign_randomized_sample_min_gp __asm__("D_L00_001619C0") __attribute__((sda));
extern int smoke_fire_emitted_particle_sign_randomized_sample_max_gp __asm__("D_L00_001619C4") __attribute__((sda));
extern int smoke_fire_emitted_particle_base_color_gp __asm__("D_L00_001619C8") __attribute__((sda));
extern float smoke_fire_far_culling_distance_gp __asm__("D_L00_001619E0") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_VELDIN_SHIP_GLOBALS_H */
