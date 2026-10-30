#ifndef LOMBYTE_RNC_OVERLAY_L10_MOBYS_PLATFORM_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L10_MOBYS_PLATFORM_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l10/mobys/platform.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float bombthrower_grass_yaw_offset_limit_degrees __asm__("D_L10_00161BF8");
extern float bombthrower_grass_launch_elevation_min_degrees __asm__("D_L10_00161BFC");
extern float bombthrower_grass_launch_elevation_max_degrees __asm__("D_L10_00161C00");
extern float bombthrower_grass_launch_speed_min __asm__("D_L10_00161C04");
extern float bombthrower_grass_launch_speed_max __asm__("D_L10_00161C08");
extern int   dialogue_message_duration __asm__("D_L10_0015F640");
extern int   gadgebot_telepot_camera_effect_duration_frames __asm__("D_L10_001618A0");
extern int   gadgebot_telepot_camera_spring_decay_frames __asm__("D_L10_001618A4");
extern int   gadgebot_telepot_camera_effect_remaining_frames __asm__("D_L10_001618A8");
extern float lab_door_local_y_half_extent __asm__("D_L10_00161C0C");
extern float lab_door_local_x_half_extent __asm__("D_L10_00161C10");
extern float data_processor_linked_effect_fade_duration_seconds __asm__("D_L10_00161C98");
extern f32   data_processor_linked_effect_row_uv_phase_step __asm__("D_L10_00161D60");
extern int   data_processor_linked_effect_color_at_zero_progress __asm__("D_L10_00161C90");
extern int   data_processor_linked_effect_color_at_full_progress __asm__("D_L10_00161C94");

#endif /* LOMBYTE_RNC_OVERLAY_L10_MOBYS_PLATFORM_GLOBALS_H */
