#ifndef LOMBYTE_RNC_RENDERING_DRAW_DIALOG_TEXT_GLOBALS_H
#define LOMBYTE_RNC_RENDERING_DRAW_DIALOG_TEXT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/rendering/draw_dialog_text.c. */
extern s32 freeze_fading_dialog_text_base_color __asm__("D_0015F4F0") __attribute__((sda));
extern s32 freeze_fading_dialog_text_blend_target_color __asm__("D_0015F4F4") __attribute__((sda));
extern s32 freeze_three_line_outline_center_x __asm__("D_0015F4F8") __attribute__((sda));
extern s32 freeze_three_line_outline_center_y __asm__("D_0015F4FC") __attribute__((sda));
extern s32 freeze_three_line_outline_half_width __asm__("D_0015F500") __attribute__((sda));
extern s32 freeze_three_line_outline_half_height __asm__("D_0015F504") __attribute__((sda));
extern s32 freeze_stats_outline_center_x __asm__("D_0015F50C") __attribute__((sda));
extern s32 freeze_stats_outline_center_y __asm__("D_0015F510") __attribute__((sda));
extern s32 freeze_stats_outline_half_width __asm__("D_0015F514") __attribute__((sda));
extern s32 freeze_stats_outline_half_height __asm__("D_0015F518") __attribute__((sda));
extern s32 freeze_outline_pulse_base_color __asm__("D_0015F524") __attribute__((sda));
extern s32 freeze_outline_pulse_blend_target_color __asm__("D_0015F528") __attribute__((sda));
extern s32 freeze_shared_dialog_text_base_color __asm__("D_0015F52C") __attribute__((sda));
extern s32 freeze_shared_dialog_text_blend_target_color __asm__("D_0015F530") __attribute__((sda));
extern s32 freeze_matched_record_text_base_color __asm__("D_0015F534") __attribute__((sda));
extern s32 freeze_matched_record_text_blend_target_color __asm__("D_0015F538") __attribute__((sda));
extern s32 freeze_unmatched_record_text_base_color __asm__("D_0015F53C") __attribute__((sda));
extern s32 freeze_unmatched_record_text_blend_target_color __asm__("D_0015F540") __attribute__((sda));

#endif /* LOMBYTE_RNC_RENDERING_DRAW_DIALOG_TEXT_GLOBALS_H */
