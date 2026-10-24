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

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_BLARG_SPACE_COMMANDO_GLOBALS_H */
