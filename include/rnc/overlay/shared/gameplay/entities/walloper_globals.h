#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_WALLOPER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_WALLOPER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/walloper.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern f32 beam_first_pass_texture_u_offset __asm__("D_L00_001615EC");
extern f32 beam_first_pass_texture_u_scroll_speed __asm__("D_L00_001615F0");
extern s32 primary_beam_first_pass_alpha __asm__("D_L00_001615D4");
extern s32 primary_beam_second_pass_alpha __asm__("D_L00_001615DC");
extern f32 beam_and_arc_first_pass_half_width __asm__("D_L00_001615FC");
extern f32 primary_beam_second_pass_half_width __asm__("D_L00_00161600");
extern f32 primary_beam_second_pass_segment_extension_factor __asm__("D_L00_00161630");
extern s32 secondary_beam_first_pass_alpha __asm__("D_L00_00161604");
extern s32 secondary_beam_second_pass_alpha __asm__("D_L00_00161608");
extern f32 secondary_beam_and_arc_second_pass_half_width __asm__("D_L00_00161610");
extern f32 secondary_beam_second_pass_segment_extension_factor __asm__("D_L00_00161634");
extern s16 beam_arc_second_row_alpha __asm__("D_L00_00161662");
extern s16 beam_arc_third_row_alpha __asm__("D_L00_00161664");
extern s16 beam_arc_fourth_row_alpha __asm__("D_L00_00161666");
extern float beam_anchor_first_pass_base_corner_scale __asm__("D_L00_00161648");
extern float beam_anchor_second_pass_base_corner_scale __asm__("D_L00_0016164C");
extern f32 walloper_arc_first_pass_half_width __asm__("D_L00_00161690");
extern f32 walloper_arc_second_pass_half_width __asm__("D_L00_00161694");
extern float walloper_quad_basis_offset_scale __asm__("D_L00_001616BC");
extern float walloper_quad_corner_scale __asm__("D_L00_001616B8");
extern int walloper_quad_texture_index __asm__("D_L00_001616C8");
extern float morphoray_base_drain_rate __asm__("D_L00_001616E0");
extern int morphoray_point_light_slot __asm__("D_L00_001617C0");
extern int morphoray_light_fade_countdown __asm__("D_L00_001617C4");
extern int beam_pulse_default_rgb __asm__("D_L00_00161760");
extern float beam_origin_quad_basis_offset_length __asm__("D_L00_001617B4");
extern float beam_origin_quad_first_pass_corner_scale __asm__("D_L00_001617AC");
extern float beam_origin_quad_second_pass_corner_scale __asm__("D_L00_001617B0");
extern int beam_origin_quad_first_pass_vertex_color __asm__("D_L00_001617B8");

/* Additional scalar data read by this unit. */
extern int beam_and_arc_first_pass_texture_index_gp __asm__("D_L00_00161638") __attribute__((sda));
extern int beam_and_arc_second_pass_texture_index_gp __asm__("D_L00_0016163C") __attribute__((sda));
extern int beam_and_arc_first_pass_base_vertex_color_gp __asm__("D_L00_001615F4") __attribute__((sda));
extern float walloper_effect_alpha_scale __asm__("D_L00_001616D4");
extern int walloper_effect_fade_out_countdown __asm__("D_L00_001616D0");
extern float morphoray_target_angle_threshold_degrees_gp __asm__("D_L00_001616D8") __attribute__((sda));
extern int beam_pulse_spawn_interval_frames_gp __asm__("D_L00_00161744") __attribute__((sda));
extern float beam_ring_band_texture_u_step_gp __asm__("D_L00_00161710") __attribute__((sda));
extern float beam_ring_face_texture_v_step_gp __asm__("D_L00_00161714") __attribute__((sda));
extern float beam_ring_texture_u_scroll_speed_gp __asm__("D_L00_0016171C") __attribute__((sda));
extern float beam_ring_texture_v_scroll_speed_gp __asm__("D_L00_00161728") __attribute__((sda));
extern int beam_rail_first_pass_alpha_gp __asm__("D_L00_0016176C") __attribute__((sda));
extern int beam_rail_second_pass_alpha_gp __asm__("D_L00_00161770") __attribute__((sda));
extern float beam_rail_first_pass_half_width_gp __asm__("D_L00_00161774") __attribute__((sda));
extern float beam_rail_second_pass_half_width_gp __asm__("D_L00_00161778") __attribute__((sda));
extern float beam_rail_second_pass_segment_extension_factor_gp __asm__("D_L00_0016177C") __attribute__((sda));
extern int beam_first_rail_second_pass_base_vertex_color_gp __asm__("D_L00_00161784") __attribute__((sda));
extern int beam_second_rail_second_pass_base_vertex_color_gp __asm__("D_L00_00161788") __attribute__((sda));
extern float beam_rail_first_pass_texture_u_offset_gp __asm__("D_L00_0016178C") __attribute__((sda));
extern float beam_rail_first_pass_texture_u_scroll_speed_gp __asm__("D_L00_00161790") __attribute__((sda));
extern int beam_ring_default_vertex_color_gp __asm__("D_L00_00161794") __attribute__((sda));
extern int beam_ring_texture_index_gp __asm__("D_L00_0016179C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_WALLOPER_GLOBALS_H */
