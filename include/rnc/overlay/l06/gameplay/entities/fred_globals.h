#ifndef LOMBYTE_RNC_OVERLAY_L06_GAMEPLAY_ENTITIES_FRED_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L06_GAMEPLAY_ENTITIES_FRED_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l06/gameplay/entities/fred.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int dual_texture_mesh_first_pass_texture_index __asm__("D_L06_00161F90");
extern int dual_texture_mesh_second_pass_vertex_color __asm__("D_L06_00161F98");
extern float dual_texture_mesh_position_scale __asm__("D_L06_00161F9C");
extern float dual_texture_mesh_first_pass_u_offset_multiplier __asm__("D_L06_00161FA0");
extern float dual_texture_mesh_first_pass_v_offset_multiplier __asm__("D_L06_00161FA4");
extern int scripted_camera_sequence_active __asm__("D_L06_0015F404");
extern float camera_sequence_moby_scale_multiplier __asm__("D_L06_00162048");
extern float camera_sequence_rotation_x_degrees __asm__("D_L06_00162050");
extern float moby_breakup_z_offset __asm__("D_L06_0016204C");
extern float moby_bobbing_amplitude __asm__("D_L06_00162090");
extern float linked_sequence_exit_ramp_progress __asm__("D_L06_0015F400");
extern s32 chatter_second_variant_used __asm__("D_L06_001620BC");
extern s32 chatter_third_variant_used __asm__("D_L06_001620C0");
extern int shuttle_forced_return_requested __asm__("D_L06_00162100");
extern float shuttle_breakup_particle_max_speed __asm__("D_L06_0016215C");
extern int shuttle_breakup_particles_per_piece __asm__("D_L06_00162190");
extern int red_button_particle_emission_count __asm__("D_L06_001621B8");
extern float vector_stream_collision_local_x_offset __asm__("D_L06_001621E8");
extern float vector_stream_collision_local_y_offset __asm__("D_L06_001621EC");
extern float vector_stream_collision_local_z_offset __asm__("D_L06_001621F0");
extern int vector_stream_blend_color_b_selector __asm__("D_L06_001621C4");
extern int vector_stream_blend_alpha_c_selector __asm__("D_L06_001621C8");
extern int vector_stream_blend_color_a_selector __asm__("D_L06_001621C0");
extern int vector_stream_blend_color_d_selector __asm__("D_L06_001621CC");
extern int vector_stream_timer_threshold_color __asm__("D_L06_001621E0");
extern int vector_stream_cycle_boundary_color __asm__("D_L06_001621E4");
extern float vector_stream_texture_u_step __asm__("D_L06_001621F4");

/* Additional scalar data read by this unit. */
extern int dual_texture_mesh_blend_color_a_selector_gp __asm__("D_L06_00161F7C") __attribute__((sda));
extern int dual_texture_mesh_blend_color_b_selector_gp __asm__("D_L06_00161F80") __attribute__((sda));
extern int dual_texture_mesh_blend_alpha_c_selector_gp __asm__("D_L06_00161F84") __attribute__((sda));
extern int dual_texture_mesh_blend_color_d_selector_gp __asm__("D_L06_00161F88") __attribute__((sda));
extern int dual_texture_mesh_blend_fixed_alpha_gp __asm__("D_L06_00161F8C") __attribute__((sda));
extern int camera_sequence_scalar_ramp_duration_frames_gp __asm__("D_L06_00162044") __attribute__((sda));
extern int backdrop_mesh_blend_color_a_selector_gp __asm__("D_L06_00162054") __attribute__((sda));
extern int backdrop_mesh_blend_color_b_selector_gp __asm__("D_L06_00162058") __attribute__((sda));
extern int backdrop_mesh_blend_alpha_c_selector_gp __asm__("D_L06_0016205C") __attribute__((sda));
extern int backdrop_mesh_blend_color_d_selector_gp __asm__("D_L06_00162060") __attribute__((sda));
extern int backdrop_mesh_effect_texture_index_gp __asm__("D_L06_00162064") __attribute__((sda));
extern int backdrop_mesh_blend_fixed_alpha_gp __asm__("D_L06_00162068") __attribute__((sda));
extern float backdrop_mesh_vertex_position_scale_gp __asm__("D_L06_00162074") __attribute__((sda));
extern float shuttle_second_stop_ride_scale_multiplier_gp __asm__("D_L06_001620F4") __attribute__((sda));
extern int class_076a_moby_spawn_requested_gp __asm__("D_L06_00162104") __attribute__((sda));
extern int class_076a_moby_rate_input_frames_gp __asm__("D_L06_00162108") __attribute__((sda));
extern int shuttle_breakup_particle_timing_random_span_frames_gp __asm__("D_L06_00162160") __attribute__((sda));
extern int shuttle_breakup_particle_timing_base_a_frames_gp __asm__("D_L06_00162164") __attribute__((sda));
extern int shuttle_breakup_particle_timing_base_b_frames_gp __asm__("D_L06_00162168") __attribute__((sda));
extern float shuttle_breakup_particle_timing_variation_fraction_gp __asm__("D_L06_0016216C") __attribute__((sda));
extern float shuttle_breakup_particle_velocity_w_random_bound_a_gp __asm__("D_L06_00162170") __attribute__((sda));
extern float shuttle_breakup_particle_velocity_w_random_bound_b_gp __asm__("D_L06_00162174") __attribute__((sda));
extern float shuttle_breakup_particle_acceleration_w_random_bound_a_gp __asm__("D_L06_00162178") __attribute__((sda));
extern float shuttle_breakup_particle_acceleration_w_random_bound_b_gp __asm__("D_L06_0016217C") __attribute__((sda));
extern int shuttle_breakup_particle_first_color_blend_a_gp __asm__("D_L06_00162180") __attribute__((sda));
extern int shuttle_breakup_particle_first_color_blend_b_gp __asm__("D_L06_00162184") __attribute__((sda));
extern int shuttle_breakup_particle_second_color_blend_a_gp __asm__("D_L06_00162188") __attribute__((sda));
extern int shuttle_breakup_particle_second_color_blend_b_gp __asm__("D_L06_0016218C") __attribute__((sda));
extern int shuttle_breakup_particle_table_index_and_flags_gp __asm__("D_L06_00162194") __attribute__((sda));
extern float vector_stream_increment_random_scale_bound_gp __asm__("D_L06_00162200") __attribute__((sda));
extern float vector_stream_phase_rate_gp __asm__("D_L06_001621FC") __attribute__((sda));
extern float vector_stream_x_extent_gp __asm__("D_L06_00162204") __attribute__((sda));
extern float vector_stream_moby_scale_multiplier_gp __asm__("D_L06_00162210") __attribute__((sda));
extern int vector_stream_burst_color_blend_a_gp __asm__("D_L06_00162214") __attribute__((sda));
extern int vector_stream_burst_color_blend_b_gp __asm__("D_L06_00162218") __attribute__((sda));
extern float vector_stream_burst_min_particle_scale_gp __asm__("D_L06_0016221C") __attribute__((sda));
extern float vector_stream_burst_max_particle_scale_gp __asm__("D_L06_00162220") __attribute__((sda));
extern int vector_stream_burst_min_lifetime_frames_gp __asm__("D_L06_00162224") __attribute__((sda));
extern int vector_stream_burst_max_lifetime_frames_gp __asm__("D_L06_00162228") __attribute__((sda));
extern float vector_stream_burst_velocity_random_scale_bound_a_gp __asm__("D_L06_0016222C") __attribute__((sda));
extern float vector_stream_burst_velocity_random_scale_bound_b_gp __asm__("D_L06_00162230") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L06_GAMEPLAY_ENTITIES_FRED_GLOBALS_H */
