#ifndef LOMBYTE_RNC_OVERLAY_SHARED_UI_HUD_DRAW_TRICK_COMBO_HUD_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_UI_HUD_DRAW_TRICK_COMBO_HUD_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/ui/hud/draw_trick_combo_hud.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern u8  trick_combo_hud_delay_reload __asm__("D_L05_0015F87C");
extern s32 trick_combo_result_text_color __asm__("D_L05_0015F8A0");
extern s32 trick_combo_pulsing_label_x __asm__("D_L05_0015F8A4");
extern s32 trick_combo_pulsing_label_y __asm__("D_L05_0015F8A8");
extern s32 trick_combo_rotation_text_center_x __asm__("D_L05_0015F8B4");
extern s32 trick_combo_rotation_text_y __asm__("D_L05_0015F8B8");
extern s32 trick_combo_pose_count_center_x __asm__("D_L05_0015F8BC");
extern s32 trick_combo_pose_count_y __asm__("D_L05_0015F8C0");
extern s32 trick_combo_reward_text_x __asm__("D_L05_0015F8C4");
extern s32 trick_combo_reward_label_y __asm__("D_L05_0015F8C8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_UI_HUD_DRAW_TRICK_COMBO_HUD_GLOBALS_H */
