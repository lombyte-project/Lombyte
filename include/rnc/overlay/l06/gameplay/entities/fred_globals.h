#ifndef LOMBYTE_RNC_OVERLAY_L06_GAMEPLAY_ENTITIES_FRED_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L06_GAMEPLAY_ENTITIES_FRED_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l06/gameplay/entities/fred.c. */
extern int dual_texture_mesh_first_pass_texture_index __asm__("D_L06_00161F90") __attribute__((sda));
extern int dual_texture_mesh_second_pass_vertex_color __asm__("D_L06_00161F98") __attribute__((sda));
extern float dual_texture_mesh_position_scale __asm__("D_L06_00161F9C") __attribute__((sda));
extern float dual_texture_mesh_first_pass_u_offset_multiplier __asm__("D_L06_00161FA0") __attribute__((sda));
extern float dual_texture_mesh_first_pass_v_offset_multiplier __asm__("D_L06_00161FA4") __attribute__((sda));
extern int scripted_camera_sequence_active __asm__("D_L06_0015F404");
extern float camera_sequence_moby_scale_multiplier __asm__("D_L06_00162048") __attribute__((sda));
extern float camera_sequence_rotation_x_degrees __asm__("D_L06_00162050") __attribute__((sda));
extern float moby_breakup_z_offset __asm__("D_L06_0016204C") __attribute__((sda));
extern float moby_bobbing_amplitude __asm__("D_L06_00162090") __attribute__((sda));
extern float linked_sequence_exit_ramp_progress __asm__("D_L06_0015F400");
extern s32 chatter_second_variant_used __asm__("D_L06_001620BC");
extern s32 chatter_third_variant_used __asm__("D_L06_001620C0");
extern int shuttle_forced_return_requested __asm__("D_L06_00162100") __attribute__((sda));
extern float shuttle_breakup_particle_max_speed __asm__("D_L06_0016215C") __attribute__((sda));
extern int shuttle_breakup_particles_per_piece __asm__("D_L06_00162190") __attribute__((sda));
extern int red_button_particle_emission_count __asm__("D_L06_001621B8") __attribute__((sda));
extern float vector_stream_collision_local_x_offset __asm__("D_L06_001621E8") __attribute__((sda));
extern float vector_stream_collision_local_y_offset __asm__("D_L06_001621EC") __attribute__((sda));
extern float vector_stream_collision_local_z_offset __asm__("D_L06_001621F0") __attribute__((sda));
extern int vector_stream_blend_color_b_selector __asm__("D_L06_001621C4") __attribute__((sda));
extern int vector_stream_blend_alpha_c_selector __asm__("D_L06_001621C8") __attribute__((sda));
extern int vector_stream_blend_color_a_selector __asm__("D_L06_001621C0") __attribute__((sda));
extern int vector_stream_blend_color_d_selector __asm__("D_L06_001621CC") __attribute__((sda));
extern int vector_stream_timer_threshold_color __asm__("D_L06_001621E0") __attribute__((sda));
extern int vector_stream_cycle_boundary_color __asm__("D_L06_001621E4") __attribute__((sda));
extern float vector_stream_texture_u_step __asm__("D_L06_001621F4") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L06_GAMEPLAY_ENTITIES_FRED_GLOBALS_H */
