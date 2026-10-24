#ifndef LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_JET_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_JET_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l14/gameplay/entities/jet.c. */
extern s32 shuttle_enter_riding_transition_frames __asm__("D_L14_00161B8C") __attribute__((sda));
extern s32 shuttle_exit_riding_transition_frames __asm__("D_L14_00161B90") __attribute__((sda));
extern f32 shuttle_horizontal_rider_correction_limit_scale __asm__("D_L14_00161B88") __attribute__((sda));
extern f32 wind_effect_strength __asm__("D_L14_00161C08") __attribute__((sda));
extern s32 wind_push_decay_frames __asm__("D_L14_00161C04") __attribute__((sda));
extern int wind_spark_speed_range_start __asm__("D_L14_00161C30") __attribute__((sda));
extern int wind_spark_speed_range_end __asm__("D_L14_00161C34") __attribute__((sda));
extern float wind_spark_size_range_start __asm__("D_L14_00161C38") __attribute__((sda));
extern float wind_spark_size_range_end __asm__("D_L14_00161C3C") __attribute__((sda));
extern int wind_spark_peak_alpha __asm__("D_L14_00161C10") __attribute__((sda));
extern int wind_spark_fade_in_frames __asm__("D_L14_00161C2C") __attribute__((sda));
extern int wind_spark_base_color __asm__("D_L14_00161C0C") __attribute__((sda));
extern f32 mine_path_step_rate __asm__("D_L14_00161C44") __attribute__((sda));
extern f32 mine_glow_pulse_phase_step __asm__("D_L14_00161C48") __attribute__((sda));
extern int lightning_arc_remaining_frames __asm__("D_L14_00161CB4");
extern float lightning_displaced_endpoint_y __asm__("D_L14_00161CD4");
extern float lightning_displaced_endpoint_z __asm__("D_L14_00161CD8");
extern float lightning_path_endpoint_w __asm__("D_L14_00161CCC");
extern f32 lightning_arc_alpha_pulse_angle_step __asm__("D_L14_00161C68") __attribute__((sda));
extern f32 lightning_arc_alpha_pulse_sweep __asm__("D_L14_00161C6C") __attribute__((sda));
extern f32 lightning_main_arc_point_spacing __asm__("D_L14_00161C74") __attribute__((sda));
extern f32 lightning_main_arc_z_displacement_limit __asm__("D_L14_00161C94") __attribute__((sda));
extern s32 lightning_arc_lifetime_range_start __asm__("D_L14_00161C98") __attribute__((sda));
extern s32 lightning_arc_lifetime_range_end __asm__("D_L14_00161C9C") __attribute__((sda));
extern s16 lightning_arc_alpha_pulse_offset __asm__("D_L14_00161CE2");
extern f32 lightning_arc_alpha_pulse_phase __asm__("D_L14_00161CE4");
extern s32 lightning_arc_initial_lifetime_frames __asm__("D_L14_00161CEC");
extern f32 lightning_arc_inverse_fade_duration __asm__("D_L14_00161CF0");
extern s32 lightning_arc_sound_pending __asm__("D_L14_00161CF4");
extern s32 moby_group_index_limit __asm__("D_L14_0015FFF4");
extern int enemy_fighter_destruction_count __asm__("D_0015EE08");
extern s32 enemy_fighter_respawn_delay_frames __asm__("D_L14_00161F5C") __attribute__((sda));
extern s32 enemy_fighter_921_respawn_delay_frames __asm__("D_L14_00161F8C") __attribute__((sda));
extern f32 moby_exhaust_base_row0_offset __asm__("D_L14_00161F60") __attribute__((sda));
extern f32 moby_exhaust_paired_emitter_offset __asm__("D_L14_00161F64") __attribute__((sda));
extern f32 moby_exhaust_base_row2_offset __asm__("D_L14_00161F68") __attribute__((sda));
extern f32 moby_exhaust_ring_row0_offset __asm__("D_L14_00161F6C") __attribute__((sda));
extern f32 moby_exhaust_grey_puff_row0_offset __asm__("D_L14_00161F70") __attribute__((sda));
extern float flight_path_spring_strength __asm__("D_L14_00161F7C") __attribute__((sda));
extern float flight_path_spring_damping __asm__("D_L14_00161F80") __attribute__((sda));
extern float flight_path_bank_yaw_error_scale __asm__("D_L14_00161F88") __attribute__((sda));
extern float flight_path_step_rate __asm__("D_L14_00161F78") __attribute__((sda));
extern s32 rocket_initial_timer_frames __asm__("D_L14_00161FB0") __attribute__((sda));
extern f32 rocket_travel_speed __asm__("D_L14_00161FA8") __attribute__((sda));
extern f32 rocket_steering_blend_fraction __asm__("D_L14_00161FAC") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_JET_GLOBALS_H */
