#ifndef LOMBYTE_RNC_OVERLAY_L11_GAMEPLAY_ENTITIES_OIL_TANKER_FRONT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L11_GAMEPLAY_ENTITIES_OIL_TANKER_FRONT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l11/gameplay/entities/oil_tanker_front.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float puffoid_hit_knockback_vertical_scale __asm__("D_L11_001622D4");
extern float puffoid_death_knockback_vertical_scale __asm__("D_L11_001622DC");
extern float gate_door_z_offset __asm__("D_L11_00162308");
extern float gate_door_x_rotation_degrees __asm__("D_L11_0016230C");
extern float gate_door_y_rotation_degrees __asm__("D_L11_00162310");
extern float oil_tanker_path_progress_step __asm__("D_L11_00162320");
extern float oil_tanker_head_fragment_velocity_scale __asm__("D_L11_00162330");
extern int layered_effect_texture_2c_fixed_alpha __asm__("D_L11_001624F8");
extern int layered_effect_texture_29_fixed_alpha_scale __asm__("D_L11_001624FC");
extern int layered_effect_texture_2a_pair_zero_fixed_alpha __asm__("D_L11_00162518");
extern int layered_effect_texture_2a_pair_one_fixed_alpha __asm__("D_L11_0016251C");
extern int layered_effect_texture_2b_fixed_alpha __asm__("D_L11_00162528");
extern float layered_effect_position_x __asm__("D_L11_001624E8");
extern float layered_effect_position_y __asm__("D_L11_001624EC");
extern float layered_effect_position_z __asm__("D_L11_001624F0");
extern int layered_effect_color_tables_initialized __asm__("D_L11_0016252C");
extern int distance_faded_effect_texture_29_fixed_alpha_scale __asm__("D_L11_001625CC");
extern int distance_faded_effect_texture_2a_pair_one_fixed_alpha __asm__("D_L11_001625EC");
extern float distance_faded_effect_position_x __asm__("D_L11_001625B8");
extern float distance_faded_effect_position_y __asm__("D_L11_001625BC");
extern float distance_faded_effect_position_z __asm__("D_L11_001625C0");
extern int distance_faded_effect_color_tables_initialized __asm__("D_L11_001625FC");

/* Additional scalar data read by this unit. */
extern float puffoid_hit_motion_time_squared_scale_gp __asm__("D_L11_001622D0") __attribute__((sda));
extern float puffoid_hit_knockback_horizontal_scale_gp __asm__("D_L11_001622D8") __attribute__((sda));
extern float puffoid_death_knockback_horizontal_scale_gp __asm__("D_L11_001622E0") __attribute__((sda));
extern float oil_tanker_link_spacing_threshold_gp __asm__("D_L11_00162324") __attribute__((sda));
extern float oil_tanker_spacing_reference_offset_gp __asm__("D_L11_00162328") __attribute__((sda));
extern s32 oil_tanker_link_marker_cooldown_frames_gp __asm__("D_L11_0016232C") __attribute__((sda));
extern float layered_effect_bounds_radius __asm__("D_L11_001624F4");
extern float distance_faded_effect_bounds_radius __asm__("D_L11_001625C4");

#endif /* LOMBYTE_RNC_OVERLAY_L11_GAMEPLAY_ENTITIES_OIL_TANKER_FRONT_GLOBALS_H */
