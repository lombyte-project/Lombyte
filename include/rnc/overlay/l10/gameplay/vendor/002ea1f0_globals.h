#ifndef LOMBYTE_RNC_OVERLAY_L10_GAMEPLAY_VENDOR_002EA1F0_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L10_GAMEPLAY_VENDOR_002EA1F0_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l10/gameplay/vendor/002ea1f0.c. */
extern int burst_spark_base_frame_count __asm__("D_L10_00161F84") __attribute__((sda));
extern int burst_spark_base_second_frame_count __asm__("D_L10_00161F88") __attribute__((sda));
extern int burst_spark_base_third_frame_count __asm__("D_L10_00161F8C") __attribute__((sda));
extern float burst_spark_frame_count_scale_spread __asm__("D_L10_00161F90") __attribute__((sda));
extern int burst_spark_timer_reset_min __asm__("D_L10_00161FA4") __attribute__((sda));
extern int burst_spark_timer_reset_max __asm__("D_L10_00161FA8") __attribute__((sda));
extern int burst_spark_active_duration_min __asm__("D_L10_00161FAC") __attribute__((sda));
extern int burst_spark_active_duration_max __asm__("D_L10_00161FB0") __attribute__((sda));
extern int burst_spark_pause_duration_min __asm__("D_L10_00161FB4") __attribute__((sda));
extern int burst_spark_pause_duration_max __asm__("D_L10_00161FB8") __attribute__((sda));
extern float timed_spark_vertical_vector_z_scale __asm__("D_L10_00161FBC") __attribute__((sda));
extern float timed_spark_random_vector_z_bias_scale __asm__("D_L10_00161FC0") __attribute__((sda));
extern float timed_spark_random_vector_radius_scale __asm__("D_L10_00161FC4") __attribute__((sda));
extern float timed_spark_vertical_vector_fourth_component_min __asm__("D_L10_00161FC8") __attribute__((sda));
extern float timed_spark_vertical_vector_fourth_component_max __asm__("D_L10_00161FCC") __attribute__((sda));
extern float timed_spark_random_vector_fourth_component_min __asm__("D_L10_00161FD0") __attribute__((sda));
extern float timed_spark_random_vector_fourth_component_max __asm__("D_L10_00161FD4") __attribute__((sda));
extern int timed_spark_base_frame_count __asm__("D_L10_00161FD8") __attribute__((sda));
extern int timed_spark_base_second_frame_count __asm__("D_L10_00161FDC") __attribute__((sda));
extern int timed_spark_base_third_frame_count __asm__("D_L10_00161FE0") __attribute__((sda));
extern float timed_spark_frame_count_scale_spread __asm__("D_L10_00161FE4") __attribute__((sda));
extern int timed_spark_timer_reset_min __asm__("D_L10_00161FF8") __attribute__((sda));
extern int timed_spark_timer_reset_max __asm__("D_L10_00161FFC") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L10_GAMEPLAY_VENDOR_002EA1F0_GLOBALS_H */
