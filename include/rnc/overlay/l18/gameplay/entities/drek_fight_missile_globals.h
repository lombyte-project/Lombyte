#ifndef LOMBYTE_RNC_OVERLAY_L18_GAMEPLAY_ENTITIES_DREK_FIGHT_MISSILE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L18_GAMEPLAY_ENTITIES_DREK_FIGHT_MISSILE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l18/gameplay/entities/drek_fight_missile.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int drek_missile_trail_draw_disabled __asm__("D_L18_0016223C");
extern int drek_missile_trail_blend_minuend_selector __asm__("D_L18_00162240");
extern int drek_missile_trail_blend_subtrahend_selector __asm__("D_L18_00162244");
extern int drek_missile_trail_blend_factor_selector __asm__("D_L18_00162248");
extern int drek_missile_trail_blend_addend_selector __asm__("D_L18_0016224C");
extern int drek_missile_trail_blend_start_color __asm__("D_L18_00162250");
extern int drek_missile_trail_blend_end_color __asm__("D_L18_00162254");
extern float drek_missile_moving_effect_phase_rate_degrees __asm__("D_L18_00162258");
extern float drek_missile_wait_to_explode_effect_phase_rate_degrees __asm__("D_L18_0016225C");
extern int drek_missile_effect_blend_start_color __asm__("D_L18_00162260");
extern int drek_missile_effect_blend_end_color __asm__("D_L18_00162264");
extern float drek_missile_effect_quad_scale __asm__("D_L18_00162268");
extern float drek_missile_effect_position_offset_length __asm__("D_L18_0016226C");
extern f32 glow_moby_phase_rate __asm__("D_L18_001622E0");
extern int glow_moby_phase_blend_start_color __asm__("D_L18_001622C8");
extern int glow_moby_phase_blend_end_color __asm__("D_L18_001622CC");
extern f32 glow_moby_layer_z_step __asm__("D_L18_001622D0");
extern int lift_alternate_height_requested __asm__("D_L18_001622FC");
extern int next_lift_instance_index __asm__("D_L18_001622F8");
extern float lift_texture_u_scroll_rate __asm__("D_L18_0016230C");

#endif /* LOMBYTE_RNC_OVERLAY_L18_GAMEPLAY_ENTITIES_DREK_FIGHT_MISSILE_GLOBALS_H */
