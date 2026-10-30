#ifndef LOMBYTE_RNC_OVERLAY_L06_MOBYS_FRED_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L06_MOBYS_FRED_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l06/mobys/fred.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int   dual_texture_mesh_first_pass_texture_index __asm__("D_L06_00161F90");
extern int   dual_texture_mesh_second_pass_vertex_color __asm__("D_L06_00161F98");
extern float dual_texture_mesh_position_scale __asm__("D_L06_00161F9C");
extern float dual_texture_mesh_first_pass_u_offset_multiplier __asm__("D_L06_00161FA0");
extern float dual_texture_mesh_first_pass_v_offset_multiplier __asm__("D_L06_00161FA4");
extern int   scripted_camera_sequence_active __asm__("D_L06_0015F404");
extern float camera_sequence_moby_scale_multiplier __asm__("D_L06_00162048");
extern float camera_sequence_rotation_x_degrees __asm__("D_L06_00162050");
extern float moby_breakup_z_offset __asm__("D_L06_0016204C");
extern float moby_bobbing_amplitude __asm__("D_L06_00162090");
extern float linked_sequence_exit_ramp_progress __asm__("D_L06_0015F400");
extern s32   chatter_second_variant_used __asm__("D_L06_001620BC");
extern s32   chatter_third_variant_used __asm__("D_L06_001620C0");
extern int   shuttle_forced_return_requested __asm__("D_L06_00162100");
extern float shuttle_breakup_particle_max_speed __asm__("D_L06_0016215C");
extern int   shuttle_breakup_particles_per_piece __asm__("D_L06_00162190");
extern int   red_button_particle_emission_count __asm__("D_L06_001621B8");
extern float vector_stream_collision_local_x_offset __asm__("D_L06_001621E8");
extern float vector_stream_collision_local_y_offset __asm__("D_L06_001621EC");
extern float vector_stream_collision_local_z_offset __asm__("D_L06_001621F0");
extern int   vector_stream_blend_color_b_selector __asm__("D_L06_001621C4");
extern int   vector_stream_blend_alpha_c_selector __asm__("D_L06_001621C8");
extern int   vector_stream_blend_color_a_selector __asm__("D_L06_001621C0");
extern int   vector_stream_blend_color_d_selector __asm__("D_L06_001621CC");
extern int   vector_stream_timer_threshold_color __asm__("D_L06_001621E0");
extern int   vector_stream_cycle_boundary_color __asm__("D_L06_001621E4");
extern float vector_stream_texture_u_step __asm__("D_L06_001621F4");

#endif /* LOMBYTE_RNC_OVERLAY_L06_MOBYS_FRED_GLOBALS_H */
