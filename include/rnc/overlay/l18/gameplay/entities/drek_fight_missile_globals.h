#ifndef LOMBYTE_RNC_OVERLAY_L18_GAMEPLAY_ENTITIES_DREK_FIGHT_MISSILE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L18_GAMEPLAY_ENTITIES_DREK_FIGHT_MISSILE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l18/gameplay/entities/drek_fight_missile.c. */
extern int drek_missile_trail_draw_disabled __asm__("D_L18_0016223C") __attribute__((sda));
extern int drek_missile_trail_blend_minuend_selector __asm__("D_L18_00162240") __attribute__((sda));
extern int drek_missile_trail_blend_subtrahend_selector __asm__("D_L18_00162244") __attribute__((sda));
extern int drek_missile_trail_blend_factor_selector __asm__("D_L18_00162248") __attribute__((sda));
extern int drek_missile_trail_blend_addend_selector __asm__("D_L18_0016224C") __attribute__((sda));
extern int drek_missile_trail_blend_start_color __asm__("D_L18_00162250") __attribute__((sda));
extern int drek_missile_trail_blend_end_color __asm__("D_L18_00162254") __attribute__((sda));
extern float drek_missile_moving_effect_phase_rate_degrees __asm__("D_L18_00162258") __attribute__((sda));
extern float drek_missile_wait_to_explode_effect_phase_rate_degrees __asm__("D_L18_0016225C") __attribute__((sda));
extern int drek_missile_effect_blend_start_color __asm__("D_L18_00162260") __attribute__((sda));
extern int drek_missile_effect_blend_end_color __asm__("D_L18_00162264") __attribute__((sda));
extern float drek_missile_effect_quad_scale __asm__("D_L18_00162268") __attribute__((sda));
extern float drek_missile_effect_position_offset_length __asm__("D_L18_0016226C") __attribute__((sda));
extern f32 glow_moby_phase_rate __asm__("D_L18_001622E0") __attribute__((sda));
extern int glow_moby_phase_blend_start_color __asm__("D_L18_001622C8") __attribute__((sda));
extern int glow_moby_phase_blend_end_color __asm__("D_L18_001622CC") __attribute__((sda));
extern f32 glow_moby_layer_z_step __asm__("D_L18_001622D0") __attribute__((sda));
extern int lift_alternate_height_requested __asm__("D_L18_001622FC") __attribute__((sda));
extern int next_lift_instance_index __asm__("D_L18_001622F8") __attribute__((sda));
extern float lift_texture_u_scroll_rate __asm__("D_L18_0016230C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L18_GAMEPLAY_ENTITIES_DREK_FIGHT_MISSILE_GLOBALS_H */
