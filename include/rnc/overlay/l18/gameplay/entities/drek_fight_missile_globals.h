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

/* Additional scalar data read by this unit. */
extern float group_transform_debris_scale_multiplier_gp __asm__("D_L18_001622C0") __attribute__((sda));
extern int group_transform_advance_enabled_gp __asm__("D_L18_00162288") __attribute__((sda));
extern float group_transform_initial_acceleration_gp __asm__("D_L18_00162290") __attribute__((sda));
extern float group_transform_initial_braking_gp __asm__("D_L18_00162294") __attribute__((sda));
extern float group_transform_initial_maximum_speed_gp __asm__("D_L18_00162298") __attribute__((sda));
extern float group_transform_later_acceleration_gp __asm__("D_L18_0016229C") __attribute__((sda));
extern float group_transform_later_braking_gp __asm__("D_L18_001622A0") __attribute__((sda));
extern float group_transform_later_maximum_speed_gp __asm__("D_L18_001622A4") __attribute__((sda));
extern float group_transform_fall_progress_threshold_gp __asm__("D_L18_001622B4") __attribute__((sda));
extern float group_transform_debris_start_progress_threshold_gp __asm__("D_L18_001622B8") __attribute__((sda));
extern int group_transform_debris_initial_random_bound_gp __asm__("D_L18_001622BC") __attribute__((sda));
extern int glow_moby_blend_minuend_selector_gp __asm__("D_L18_001622E4") __attribute__((sda));
extern int glow_moby_blend_subtrahend_selector_gp __asm__("D_L18_001622E8") __attribute__((sda));
extern int glow_moby_blend_factor_selector_gp __asm__("D_L18_001622EC") __attribute__((sda));
extern int glow_moby_blend_addend_selector_gp __asm__("D_L18_001622F0") __attribute__((sda));
extern int drek_boss_arena_transition_frame_count_gp __asm__("D_L18_00162388") __attribute__((sda));
extern int drek_boss_arena_phase_reset_requested_gp __asm__("D_L18_0016238C") __attribute__((sda));
extern f32 drek_boss_circling_default_near_hero_speed_gp __asm__("D_L18_00162350") __attribute__((sda));
extern f32 drek_boss_circling_default_far_hero_speed_gp __asm__("D_L18_00162354") __attribute__((sda));
extern f32 drek_boss_circling_base_far_hero_distance_gp __asm__("D_L18_00162358") __attribute__((sda));
extern f32 drek_boss_circling_base_near_hero_distance_gp __asm__("D_L18_0016235C") __attribute__((sda));
extern f32 drek_boss_circle_and_fire_radius_gp __asm__("D_L18_00162370") __attribute__((sda));
extern f32 drek_boss_default_circling_radius_gp __asm__("D_L18_00162374") __attribute__((sda));
extern float drek_boss_scalar_maximum_speed_gp __asm__("D_L18_001623B0") __attribute__((sda));
extern float drek_boss_scalar_blend_min_hero_distance_gp __asm__("D_L18_001623B8") __attribute__((sda));
extern float drek_boss_scalar_blend_max_hero_distance_gp __asm__("D_L18_001623BC") __attribute__((sda));
extern float drek_boss_first_scalar_far_hero_target_gp __asm__("D_L18_001623C0") __attribute__((sda));
extern float drek_boss_second_scalar_far_hero_target_gp __asm__("D_L18_001623C4") __attribute__((sda));
extern float drek_boss_first_scalar_near_hero_target_gp __asm__("D_L18_001623C8") __attribute__((sda));
extern float drek_boss_first_scalar_gated_follow_target_gp __asm__("D_L18_001623D0") __attribute__((sda));
extern float drek_boss_second_scalar_gated_follow_target_gp __asm__("D_L18_001623D4") __attribute__((sda));
extern float drek_boss_first_scalar_near_node_follow_target_gp __asm__("D_L18_001623D8") __attribute__((sda));
extern float drek_boss_follow_camera_distance_target_gp __asm__("D_L18_001623E0") __attribute__((sda));
extern float drek_boss_second_scalar_near_hero_target_gp __asm__("D_L18_001623CC") __attribute__((sda));
extern float drek_boss_follow_camera_height_target_gp __asm__("D_L18_001623E4") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L18_GAMEPLAY_ENTITIES_DREK_FIGHT_MISSILE_GLOBALS_H */
