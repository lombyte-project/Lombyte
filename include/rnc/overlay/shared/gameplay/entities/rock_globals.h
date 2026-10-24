#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_ROCK_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_ROCK_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/rock.c. */
extern float cutscene_big_head_scale __asm__("D_L01_00161D30") __attribute__((sda));
extern s32 visibomb_outside_zone_overlay_enabled __asm__("D_L01_0015F30C") __attribute__((sda));
extern f32 gold_bolt_spin_x_degrees_per_second __asm__("D_L01_00161F20") __attribute__((sda));
extern f32 gold_bolt_spin_y_degrees_per_second __asm__("D_L01_00161F24") __attribute__((sda));
extern f32 gold_bolt_spin_z_degrees_per_second __asm__("D_L01_00161F28") __attribute__((sda));
extern int gold_bolt_glow_ring_tween_start_color __asm__("D_L01_00161F3C") __attribute__((sda));
extern int gold_bolt_glow_ring_tween_end_color __asm__("D_L01_00161F40") __attribute__((sda));
extern int gold_bolt_glow_ring_particle_use_flags_44 __asm__("D_L01_00161F48") __attribute__((sda));
extern int gold_bolt_glow_ring_particle_texture_index __asm__("D_L01_00161F4C") __attribute__((sda));
extern float gold_bolt_particle_initial_speed_min __asm__("D_L01_00161F54") __attribute__((sda));
extern float gold_bolt_particle_initial_speed_max __asm__("D_L01_00161F58") __attribute__((sda));
extern float gold_bolt_particle_velocity_jitter_speed_min __asm__("D_L01_00161F5C") __attribute__((sda));
extern float gold_bolt_particle_velocity_jitter_speed_max __asm__("D_L01_00161F60") __attribute__((sda));
extern int gold_bolt_particle_initial_color __asm__("D_L01_00161F68") __attribute__((sda));
extern int gold_bolt_particle_loop_color __asm__("D_L01_00161F6C") __attribute__((sda));
extern float gold_bolt_particle_initial_scale __asm__("D_L01_00161F70") __attribute__((sda));
extern float gold_bolt_particle_loop_scale __asm__("D_L01_00161F74") __attribute__((sda));
extern float teleporter_child_motion_acceleration __asm__("D_L01_00161F88") __attribute__((sda));
extern float teleporter_child_motion_deceleration __asm__("D_L01_00161F8C") __attribute__((sda));
extern float teleporter_child_motion_max_speed __asm__("D_L01_00161F90") __attribute__((sda));
extern float teleporter_child_position_z_offset __asm__("D_L01_00161F78") __attribute__((sda));
extern float teleporter_child_displacement_angle_degrees __asm__("D_L01_00161F80") __attribute__((sda));
extern int moby_radius_ring_blend_minuend_selector __asm__("D_L01_00161FB8") __attribute__((sda));
extern int moby_radius_ring_blend_subtrahend_selector __asm__("D_L01_00161FBC") __attribute__((sda));
extern int moby_radius_ring_blend_alpha_factor_selector __asm__("D_L01_00161FC0") __attribute__((sda));
extern int moby_radius_ring_blend_addend_selector __asm__("D_L01_00161FC4") __attribute__((sda));
extern int moby_radius_ring_texture_index __asm__("D_L01_00161FC8") __attribute__((sda));
extern int moby_radius_ring_vertex_color __asm__("D_L01_00161FCC") __attribute__((sda));
extern float moby_radius_ring_center_z __asm__("D_L01_00161FD0") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_ROCK_GLOBALS_H */
