#ifndef LOMBYTE_RNC_OVERLAY_SHARED_UI_TEXT_00262AE8_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_UI_TEXT_00262AE8_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/ui/text/00262ae8.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern u8 trick_combo_hud_delay_reload __asm__("D_L05_0015F87C");
extern s32 trick_combo_result_text_color __asm__("D_L05_0015F8A0");
extern s32 trick_combo_pulsing_label_x __asm__("D_L05_0015F8A4");
extern s32 trick_combo_pulsing_label_y __asm__("D_L05_0015F8A8");
extern s32 trick_combo_rotation_text_center_x __asm__("D_L05_0015F8B4");
extern s32 trick_combo_rotation_text_y __asm__("D_L05_0015F8B8");
extern s32 trick_combo_pose_count_center_x __asm__("D_L05_0015F8BC");
extern s32 trick_combo_pose_count_y __asm__("D_L05_0015F8C0");
extern s32 trick_combo_reward_text_x __asm__("D_L05_0015F8C4");
extern s32 trick_combo_reward_label_y __asm__("D_L05_0015F8C8");

/* Additional scalar data read by this unit. */
extern int animation_parameter_target_id __asm__("D_L05_0015F980");
extern s32 trick_combo_text_color_at_zero_delay_gp __asm__("D_L05_0015F894") __attribute__((sda));
extern s32 trick_combo_text_color_at_full_delay_gp __asm__("D_L05_0015F898") __attribute__((sda));
extern s32 trick_combo_pulsing_label_trough_color_gp __asm__("D_L05_0015F8AC") __attribute__((sda));
extern s32 trick_combo_pulsing_label_peak_color_gp __asm__("D_L05_0015F8B0") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_UI_TEXT_00262AE8_GLOBALS_H */
