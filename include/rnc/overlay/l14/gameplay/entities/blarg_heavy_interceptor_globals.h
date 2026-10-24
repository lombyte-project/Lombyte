#ifndef LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_BLARG_HEAVY_INTERCEPTOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_BLARG_HEAVY_INTERCEPTOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l14/gameplay/entities/blarg_heavy_interceptor.c. */
extern f32 blarg_heavy_interceptor_culling_sphere_radius __asm__("D_L14_0016139C") __attribute__((sda));
extern s32 blarg_heavy_interceptor_light_ring_vertex_count __asm__("D_L14_00161418") __attribute__((sda));
extern float blarg_heavy_interceptor_spinning_beam_initial_offset_scale __asm__("D_L14_00161434") __attribute__((sda));
extern float blarg_heavy_interceptor_spinning_beam_offset_scale_increment_per_pair __asm__("D_L14_00161438") __attribute__((sda));
extern float blarg_heavy_interceptor_spinning_beam_initial_quad_scale __asm__("D_L14_0016143C") __attribute__((sda));
extern float blarg_heavy_interceptor_spinning_beam_quad_scale_increment_per_pair __asm__("D_L14_00161440") __attribute__((sda));
extern int blarg_heavy_interceptor_spinning_beam_initial_red __asm__("D_L14_00161444") __attribute__((sda));
extern int blarg_heavy_interceptor_spinning_beam_initial_green __asm__("D_L14_00161448") __attribute__((sda));
extern int blarg_heavy_interceptor_spinning_beam_initial_blue __asm__("D_L14_0016144C") __attribute__((sda));
extern int blarg_heavy_interceptor_spinning_beam_initial_alpha __asm__("D_L14_00161450") __attribute__((sda));
extern int blarg_heavy_interceptor_spinning_beam_red_decrement_per_pair __asm__("D_L14_00161454") __attribute__((sda));
extern int blarg_heavy_interceptor_spinning_beam_green_decrement_per_pair __asm__("D_L14_00161458") __attribute__((sda));
extern int blarg_heavy_interceptor_spinning_beam_blue_decrement_per_pair __asm__("D_L14_0016145C") __attribute__((sda));
extern int blarg_heavy_interceptor_spinning_beam_alpha_decrement_per_pair __asm__("D_L14_00161460") __attribute__((sda));
extern float blarg_heavy_interceptor_glow_quad_scale __asm__("D_L14_00161474") __attribute__((sda));
extern float blarg_heavy_interceptor_glow_position_offset_scale __asm__("D_L14_00161478") __attribute__((sda));
extern f32 blarg_heavy_interceptor_vertical_oscillation_amplitude __asm__("D_L14_001613A8") __attribute__((sda));
extern f32 blarg_heavy_interceptor_vertical_oscillation_phase_increment __asm__("D_L14_001613AC") __attribute__((sda));
extern f32 blarg_heavy_interceptor_rotation_oscillation_amplitude __asm__("D_L14_001613B0") __attribute__((sda));
extern f32 blarg_heavy_interceptor_rotation_oscillation_first_phase_increment __asm__("D_L14_001613B4") __attribute__((sda));
extern f32 blarg_heavy_interceptor_rotation_oscillation_second_phase_increment __asm__("D_L14_001613B8") __attribute__((sda));
extern s32 blarg_heavy_interceptor_patrol_heading_change_interval_frames __asm__("D_L14_001613BC") __attribute__((sda));
extern f32 blarg_heavy_interceptor_patrol_heading_offset_magnitude __asm__("D_L14_001613C0") __attribute__((sda));
extern f32 blarg_heavy_interceptor_patrol_heading_error_gain __asm__("D_L14_001613C4") __attribute__((sda));
extern f32 blarg_heavy_interceptor_patrol_heading_damping_coefficient __asm__("D_L14_001613C8") __attribute__((sda));
extern f32 blarg_heavy_interceptor_patrol_heading_snap_threshold_scale __asm__("D_L14_001613CC") __attribute__((sda));
extern float minibomber_bomb_gravity __asm__("D_L14_00161568") __attribute__((sda));
extern f32 flying_debris_gravity __asm__("D_L14_001615D0") __attribute__((sda));
extern f32 flying_debris_velocity_retention_factor __asm__("D_L14_001615D4") __attribute__((sda));
extern f32 flying_debris_horizontal_drag_speed_threshold __asm__("D_L14_001615D8") __attribute__((sda));
extern s32 level14_racer_start_interpolation_duration_frames __asm__("D_L14_00161A2C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_BLARG_HEAVY_INTERCEPTOR_GLOBALS_H */
