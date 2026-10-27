#ifndef LOMBYTE_RNC_OVERLAY_L12_GAMEPLAY_ENTITIES_HOVEN_TURRET_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L12_GAMEPLAY_ENTITIES_HOVEN_TURRET_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l12/gameplay/entities/hoven_turret.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float oscillating_link_strip_segment_u_step __asm__("D_L12_00161DF4");
extern float oscillating_link_strip_scroll_phase __asm__("D_L12_00161DF8");
extern int oscillating_link_strip_reset_ticks __asm__("D_L12_00161E04");
extern int oscillating_link_strip_fade_pivot_ticks __asm__("D_L12_00161E08");
extern float oscillating_link_strip_scroll_rate __asm__("D_L12_00161DFC");
extern float oscillating_link_strip_random_delta_magnitude_rate __asm__("D_L12_00161E00");

/* Additional scalar data read by this unit. */
extern int oscillating_link_strip_blend_minuend_color_selector __asm__("D_L12_00161DCC") __attribute__((sda));
extern int oscillating_link_strip_blend_subtrahend_color_selector __asm__("D_L12_00161DD0") __attribute__((sda));
extern int oscillating_link_strip_blend_alpha_factor_selector __asm__("D_L12_00161DD4") __attribute__((sda));
extern int oscillating_link_strip_blend_addend_color_selector __asm__("D_L12_00161DD8") __attribute__((sda));
extern int oscillating_link_strip_fade_source_color __asm__("D_L12_00161DEC") __attribute__((sda));
extern int oscillating_link_strip_fade_target_color __asm__("D_L12_00161DF0") __attribute__((sda));
extern float hoven_turret_destruction_view_offset_distance_min __asm__("D_L12_00161E60") __attribute__((sda));
extern float hoven_turret_destruction_view_offset_distance_max __asm__("D_L12_00161E64") __attribute__((sda));
extern float hoven_turret_pitch_min __asm__("D_L12_00161EA8") __attribute__((sda));
extern float hoven_turret_pitch_max __asm__("D_L12_00161EAC") __attribute__((sda));
extern float hoven_turret_yaw_input_rate __asm__("D_L12_00161ED0") __attribute__((sda));
extern float hoven_turret_pitch_input_rate __asm__("D_L12_00161ED4") __attribute__((sda));
extern float hoven_turret_yaw_acceleration_rate __asm__("D_L12_00161ED8") __attribute__((sda));
extern float hoven_turret_pitch_acceleration_rate __asm__("D_L12_00161EDC") __attribute__((sda));
extern float hoven_turret_yaw_braking_rate __asm__("D_L12_00161EE0") __attribute__((sda));
extern float hoven_turret_pitch_braking_rate __asm__("D_L12_00161EE4") __attribute__((sda));
extern float hoven_turret_firing_offset_rate __asm__("D_L12_00161EE8") __attribute__((sda));
extern int hoven_turret_hud_health_dial_vertex_0_uv __asm__("D_L12_00161E2C") __attribute__((sda));
extern int hoven_turret_hud_health_dial_vertex_1_uv __asm__("D_L12_00161E30") __attribute__((sda));
extern int hoven_turret_hud_health_dial_vertex_2_uv __asm__("D_L12_00161E34") __attribute__((sda));
extern int hoven_turret_hud_health_dial_vertex_3_uv __asm__("D_L12_00161E38") __attribute__((sda));
extern int hoven_turret_hud_yaw_indicator_texture_index __asm__("D_L12_00161E3C") __attribute__((sda));
extern int hoven_turret_hud_pitch_indicator_texture_index __asm__("D_L12_00161E40") __attribute__((sda));
extern int hoven_turret_hud_orientation_indicator_vertex_0_uv __asm__("D_L12_00161E4C") __attribute__((sda));
extern int hoven_turret_hud_orientation_indicator_vertex_1_uv __asm__("D_L12_00161E50") __attribute__((sda));
extern int hoven_turret_hud_orientation_indicator_vertex_2_uv __asm__("D_L12_00161E54") __attribute__((sda));
extern int hoven_turret_hud_orientation_indicator_vertex_3_uv __asm__("D_L12_00161E58") __attribute__((sda));
extern int hoven_turret_hud_frame_texture_index_offset __asm__("D_L12_00161E5C") __attribute__((sda));
extern int hoven_turret_hud_frame_color __asm__("D_L12_00161EB0") __attribute__((sda));
extern int hoven_turret_hud_frame_vertex_0_uv __asm__("D_L12_00161F08") __attribute__((sda));
extern int hoven_turret_hud_frame_vertex_1_uv __asm__("D_L12_00161F0C") __attribute__((sda));
extern int hoven_turret_hud_frame_vertex_2_uv __asm__("D_L12_00161F10") __attribute__((sda));
extern int hoven_turret_hud_frame_vertex_3_uv __asm__("D_L12_00161F14") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L12_GAMEPLAY_ENTITIES_HOVEN_TURRET_GLOBALS_H */
