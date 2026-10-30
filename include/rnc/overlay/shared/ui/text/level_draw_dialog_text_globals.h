#ifndef LOMBYTE_RNC_OVERLAY_SHARED_UI_TEXT_LEVEL_DRAW_DIALOG_TEXT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_UI_TEXT_LEVEL_DRAW_DIALOG_TEXT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/ui/text/level_draw_dialog_text.c. */
extern s32 default_dialog_line1_y __asm__("D_L00_0015F4A8") __attribute__((sda));
extern s32 default_dialog_line2_y __asm__("D_L00_0015F4AC") __attribute__((sda));
extern s32 dialog_text_tween_start_color __asm__("D_L00_0015F4B0") __attribute__((sda));
extern s32 dialog_text_tween_end_color __asm__("D_L00_0015F4B4") __attribute__((sda));
extern s32 results_fixed_labels_frame_center_x __asm__("D_L00_0015F4B8") __attribute__((sda));
extern s32 results_fixed_labels_frame_center_y __asm__("D_L00_0015F4BC") __attribute__((sda));
extern s32 results_fixed_labels_frame_half_width __asm__("D_L00_0015F4C0") __attribute__((sda));
extern s32 results_fixed_labels_frame_half_height __asm__("D_L00_0015F4C4") __attribute__((sda));
extern s32 results_fixed_labels_base_y __asm__("D_L00_0015F4C8") __attribute__((sda));
extern s32 results_rank_and_records_frame_center_x __asm__("D_L00_0015F4CC") __attribute__((sda));
extern s32 results_rank_and_records_frame_center_y __asm__("D_L00_0015F4D0") __attribute__((sda));
extern s32 results_rank_and_records_frame_half_width __asm__("D_L00_0015F4D4") __attribute__((sda));
extern s32 results_rank_and_records_frame_half_height __asm__("D_L00_0015F4D8")
    __attribute__((sda));
extern s32 results_rank_and_records_base_y __asm__("D_L00_0015F4DC") __attribute__((sda));
extern s32 dialog_frame_pulse_period __asm__("D_L00_0015F4E0") __attribute__((sda));
extern s32 dialog_frame_pulse_start_color __asm__("D_L00_0015F4E4") __attribute__((sda));
extern s32 dialog_frame_pulse_end_color __asm__("D_L00_0015F4E8") __attribute__((sda));
extern s32 results_label_tween_start_color __asm__("D_L00_0015F4EC") __attribute__((sda));
extern s32 results_label_tween_end_color __asm__("D_L00_0015F4F0") __attribute__((sda));
extern s32 first_place_or_record_tween_start_color __asm__("D_L00_0015F4F4") __attribute__((sda));
extern s32 first_place_or_record_tween_end_color __asm__("D_L00_0015F4F8") __attribute__((sda));
extern s32 other_place_or_no_record_tween_start_color __asm__("D_L00_0015F4FC")
    __attribute__((sda));
extern s32 other_place_or_no_record_tween_end_color __asm__("D_L00_0015F500") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_UI_TEXT_LEVEL_DRAW_DIALOG_TEXT_GLOBALS_H */
