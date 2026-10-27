#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BLARG_SPACE_COMMANDO_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BLARG_SPACE_COMMANDO_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/blarg_space_commando.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern s32 ranged_attack_cooldown_frames __asm__("D_L06_00161FC0");
extern f32 melee_attack_distance_threshold __asm__("D_L06_00161FC4");
extern f32 vehicle_melee_windup_animation_speed __asm__("D_L06_00161FCC");
extern float beam_spread_degrees __asm__("D_L06_00161FD4");
extern int beam_duration_frames __asm__("D_L06_00161FD8");
extern f32 beam_point_horizontal_speed __asm__("D_L06_00161FD0");
extern s32 beam_effect_texture_index __asm__("D_L06_00161FF0");
extern s32 beam_outer_corner_color_at_zero_weight __asm__("D_L06_00161FF4");
extern s32 beam_outer_corner_color_at_unit_weight __asm__("D_L06_00161FF8");
extern s32 beam_inset_corner_color_at_zero_weight __asm__("D_L06_00161FFC");
extern s32 beam_inset_corner_color_at_unit_weight __asm__("D_L06_00162000");
extern float beam_spark_first_vector_w __asm__("D_L06_0016200C");
extern float beam_spark_second_vector_w __asm__("D_L06_00162010");
extern float beam_spark_frame_count_range_start __asm__("D_L06_00162014");
extern float beam_spark_frame_count_range_end __asm__("D_L06_00162018");
extern float beam_spark_second_frame_count_range_start __asm__("D_L06_0016201C");
extern float beam_spark_second_frame_count_range_end __asm__("D_L06_00162020");
extern float beam_spark_third_frame_count_range_start __asm__("D_L06_00162024");
extern float beam_spark_third_frame_count_range_end __asm__("D_L06_00162028");
extern float beam_spark_horizontal_scale_range_start __asm__("D_L06_0016202C");
extern float beam_spark_horizontal_scale_range_end __asm__("D_L06_00162030");
extern float beam_spark_vector_z_range_start __asm__("D_L06_00162034");
extern float beam_spark_vector_z_range_end __asm__("D_L06_00162038");
extern float beam_spark_spawn_probability __asm__("D_L06_0016203C");

/* Additional scalar data read by this unit. */
extern f32 movement_max_horizontal_speed_gp __asm__("D_L06_00161FC8") __attribute__((sda));
extern float base_home_distance_limit_gp __asm__("D_L06_00161FAC") __attribute__((sda));
extern float nonfatal_knockback_horizontal_speed_gp __asm__("D_L06_00161FB0") __attribute__((sda));
extern float nonfatal_knockback_vertical_speed_gp __asm__("D_L06_00161FB4") __attribute__((sda));
extern float fatal_knockback_horizontal_speed_gp __asm__("D_L06_00161FB8") __attribute__((sda));
extern float fatal_knockback_vertical_speed_gp __asm__("D_L06_00161FBC") __attribute__((sda));
extern s32 beam_blend_minuend_selector_gp __asm__("D_L06_00161FDC") __attribute__((sda));
extern s32 beam_blend_subtrahend_selector_gp __asm__("D_L06_00161FE0") __attribute__((sda));
extern s32 beam_blend_alpha_multiplier_selector_gp __asm__("D_L06_00161FE4") __attribute__((sda));
extern s32 beam_blend_addend_selector_gp __asm__("D_L06_00161FE8") __attribute__((sda));
extern s32 beam_blend_fixed_alpha_gp __asm__("D_L06_00161FEC") __attribute__((sda));
extern int beam_spark_initial_word_gp __asm__("D_L06_00162004") __attribute__((sda));
extern int beam_spark_second_word_gp __asm__("D_L06_00162008") __attribute__((sda));
extern int beam_spark_lookup_index_and_flags_gp __asm__("D_L06_00162040") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BLARG_SPACE_COMMANDO_GLOBALS_H */
