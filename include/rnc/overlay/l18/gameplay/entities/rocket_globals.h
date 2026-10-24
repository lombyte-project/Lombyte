#ifndef LOMBYTE_RNC_OVERLAY_L18_GAMEPLAY_ENTITIES_ROCKET_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L18_GAMEPLAY_ENTITIES_ROCKET_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l18/gameplay/entities/rocket.c. */
extern float floating_platform_bob_amplitude __asm__("D_L18_00161A48") __attribute__((sda));
extern float floating_platform_bob_phase_rate __asm__("D_L18_00161A4C") __attribute__((sda));
extern float class_274_ring_phase_rate __asm__("D_L18_00161CA0") __attribute__((sda));
extern float rocket_target_pulse_lead_time __asm__("D_L18_00161964");
extern float rocket_spawn_scale_multiplier __asm__("D_L18_00161960") __attribute__((sda));
extern float rocket_explosion_hit_priority __asm__("D_L18_0016196C") __attribute__((sda));
extern int rocket_target_pulse_color_start __asm__("D_L18_00161980") __attribute__((sda));
extern int rocket_target_pulse_color_end __asm__("D_L18_00161984") __attribute__((sda));
extern s32 path_link_count __asm__("D_L18_0015F710");
extern float thruster_pack_lock_green_active_height_offset __asm__("D_L18_0016199C") __attribute__((sda));
extern float thruster_pack_lock_green_rest_height_offset __asm__("D_L18_001619A0") __attribute__((sda));
extern float floating_platform_sink_trigger_distance __asm__("D_L18_00161A5C") __attribute__((sda));
extern int ballistic_ring_glow_color_end __asm__("D_L18_00161C00") __attribute__((sda));
extern float ballistic_ring_trail_spark_gravity __asm__("D_L18_00161C04") __attribute__((sda));
extern float ballistic_ring_trail_movement_step_scale __asm__("D_L18_00161C08") __attribute__((sda));
extern int ballistic_ring_trail_initial_color_start __asm__("D_L18_00161C30") __attribute__((sda));
extern int ballistic_ring_trail_initial_color_end __asm__("D_L18_00161C34");
extern int ballistic_ring_trail_second_color_start __asm__("D_L18_00161C38") __attribute__((sda));
extern int ballistic_ring_trail_second_color_end __asm__("D_L18_00161C3C") __attribute__((sda));
extern int ballistic_ring_trail_sparks_per_update __asm__("D_L18_00161C40") __attribute__((sda));
extern int expanding_projectile_ring_color_start __asm__("D_L18_00161C5C") __attribute__((sda));
extern int expanding_projectile_ring_color_end __asm__("D_L18_00161C60") __attribute__((sda));
extern float expanding_projectile_ring_segment_angle_degrees __asm__("D_L18_00161C64") __attribute__((sda));
extern float expanding_projectile_ring_draw_height_offset __asm__("D_L18_00161C68") __attribute__((sda));
extern float expanding_projectile_ring_layer_radius_offset __asm__("D_L18_00161C70") __attribute__((sda));
extern float ring_segment_particle_cull_depth_limit __asm__("D_L18_00161BF4") __attribute__((sda));
extern int ring_segment_particle_color_start __asm__("D_L18_00161C80") __attribute__((sda));
extern int ring_segment_particle_color_end __asm__("D_L18_00161C84") __attribute__((sda));
extern float ring_segment_particle_scale __asm__("D_L18_00161C88") __attribute__((sda));
extern int ring_segment_radial_particle_lifetime_frames __asm__("D_L18_00161C8C") __attribute__((sda));
extern int ring_segment_horizontal_particle_lifetime_frames __asm__("D_L18_00161C90") __attribute__((sda));
extern float ring_segment_particle_spawn_probability __asm__("D_L18_00161C98") __attribute__((sda));
extern int class_274_ring_color_start __asm__("D_L18_00161CB8") __attribute__((sda));
extern int class_274_ring_color_end __asm__("D_L18_00161CBC") __attribute__((sda));
extern float class_274_ring_segment_angle_degrees __asm__("D_L18_00161CC0") __attribute__((sda));
extern float class_274_ring_draw_height_offset __asm__("D_L18_00161CC4") __attribute__((sda));
extern float class_274_ring_layer_radius_offset __asm__("D_L18_00161CCC") __attribute__((sda));
extern int position_and_rotation_transition_active __asm__("D_L18_0015F404");
extern float electric_arc_endpoint_radius_scale __asm__("D_L18_00161ED8") __attribute__((sda));
extern float target_travel_speed __asm__("D_L18_00161EE0") __attribute__((sda));
extern int explosion_burst_spark_count __asm__("D_L18_00161F6C") __attribute__((sda));
extern int explosion_burst_spark_frame_count __asm__("D_L18_00161F80") __attribute__((sda));
extern int explosion_burst_spark_second_frame_count __asm__("D_L18_00161F84") __attribute__((sda));
extern int explosion_burst_spark_third_frame_count __asm__("D_L18_00161F88") __attribute__((sda));
extern int explosion_burst_particle_count __asm__("D_L18_00161F98") __attribute__((sda));
extern int explosion_burst_camera_shake_duration_frames __asm__("D_L18_00161FE8") __attribute__((sda));
extern float explosion_burst_spark_speed_min __asm__("D_L18_00161F70") __attribute__((sda));
extern float explosion_burst_spark_speed_max __asm__("D_L18_00161F74") __attribute__((sda));
extern float explosion_burst_spark_second_vector_scale __asm__("D_L18_00161F94") __attribute__((sda));
extern float explosion_burst_particle_speed_min __asm__("D_L18_00161FA0") __attribute__((sda));
extern float explosion_burst_particle_speed_max __asm__("D_L18_00161FA4") __attribute__((sda));
extern float explosion_burst_particle_xy_offset_limit __asm__("D_L18_00161FBC") __attribute__((sda));
extern float explosion_burst_camera_shake_amplitude __asm__("D_L18_00161FE4") __attribute__((sda));
extern int electric_bolt_initialization_requested __asm__("D_L18_00161FEC") __attribute__((sda));
extern float primary_bolt_z_phase_step __asm__("D_L18_00161FFC") __attribute__((sda));
extern float primary_bolt_z_amplitude_start __asm__("D_L18_00162000") __attribute__((sda));
extern float primary_bolt_z_amplitude_end __asm__("D_L18_00162004") __attribute__((sda));
extern float primary_bolt_z_phase_increment __asm__("D_L18_0016200C") __attribute__((sda));
extern float secondary_bolt_orbit_phase_decrement __asm__("D_L18_00162014") __attribute__((sda));
extern float secondary_bolt_orbit_phase_step __asm__("D_L18_00162018") __attribute__((sda));
extern float secondary_bolt_z_amplitude __asm__("D_L18_0016201C") __attribute__((sda));
extern float secondary_bolt_lateral_amplitude __asm__("D_L18_00162020") __attribute__((sda));
extern float secondary_bolt_z_phase_increment __asm__("D_L18_00162028") __attribute__((sda));
extern float secondary_bolt_z_phase_step __asm__("D_L18_0016202C") __attribute__((sda));
extern float primary_bolt_random_step_blend_weight __asm__("D_L18_0016208C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L18_GAMEPLAY_ENTITIES_ROCKET_GLOBALS_H */
