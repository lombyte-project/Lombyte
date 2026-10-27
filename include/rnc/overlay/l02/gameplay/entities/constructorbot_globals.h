#ifndef LOMBYTE_RNC_OVERLAY_L02_GAMEPLAY_ENTITIES_CONSTRUCTORBOT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L02_GAMEPLAY_ENTITIES_CONSTRUCTORBOT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l02/gameplay/entities/constructorbot.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern f32 shard_vertical_velocity_approach_scale __asm__("D_L02_00161F58");
extern s32 view_gated_smoke_burst_interval_min __asm__("D_L02_0016210C");
extern s32 view_gated_smoke_burst_interval_max __asm__("D_L02_00162110");
extern s32 alternating_smoke_burst_interval_min __asm__("D_L02_0016215C");
extern s32 alternating_smoke_burst_interval_max __asm__("D_L02_00162160");

/* Additional scalar data read by this unit. */
extern f32 alternating_smoke_puff_velocity_magnitude_scale_gp __asm__("D_L02_00162048") __attribute__((sda));
extern f32 alternating_smoke_puff_vertical_acceleration_scale_gp __asm__("D_L02_0016204C") __attribute__((sda));
extern f32 alternating_smoke_puff_lateral_acceleration_magnitude_bound_gp __asm__("D_L02_00162050") __attribute__((sda));
extern f32 alternating_smoke_puff_motion_variation_fraction_gp __asm__("D_L02_00162054") __attribute__((sda));
extern f32 alternating_smoke_puff_timing_variation_fraction_gp __asm__("D_L02_00162064") __attribute__((sda));
extern f32 view_gated_smoke_puff_velocity_magnitude_scale_gp __asm__("D_L02_00162088") __attribute__((sda));
extern f32 view_gated_smoke_puff_vertical_acceleration_scale_gp __asm__("D_L02_0016208C") __attribute__((sda));
extern f32 view_gated_smoke_puff_lateral_acceleration_magnitude_bound_gp __asm__("D_L02_00162090") __attribute__((sda));
extern f32 view_gated_smoke_puff_motion_variation_fraction_gp __asm__("D_L02_00162094") __attribute__((sda));
extern s32 view_gated_smoke_puff_timing_random_span_gp __asm__("D_L02_00162098") __attribute__((sda));
extern f32 view_gated_smoke_puff_timing_variation_fraction_gp __asm__("D_L02_001620A4") __attribute__((sda));
extern f32 view_gated_smoke_burst_velocity_magnitude_scale_gp __asm__("D_L02_001620C8") __attribute__((sda));
extern f32 view_gated_smoke_burst_acceleration_magnitude_scale_gp __asm__("D_L02_001620CC") __attribute__((sda));
extern f32 view_gated_smoke_burst_vertical_acceleration_decrement_scale_gp __asm__("D_L02_001620D0") __attribute__((sda));
extern f32 view_gated_smoke_burst_motion_variation_fraction_gp __asm__("D_L02_001620D4") __attribute__((sda));
extern f32 view_gated_smoke_burst_jitter_magnitude_max_gp __asm__("D_L02_001620D8") __attribute__((sda));
extern s32 view_gated_smoke_burst_timing_random_span_gp __asm__("D_L02_001620DC") __attribute__((sda));
extern f32 view_gated_smoke_burst_timing_variation_fraction_gp __asm__("D_L02_001620E8") __attribute__((sda));
extern f32 alternating_smoke_burst_velocity_magnitude_scale_gp __asm__("D_L02_00162118") __attribute__((sda));
extern f32 alternating_smoke_burst_acceleration_magnitude_scale_gp __asm__("D_L02_0016211C") __attribute__((sda));
extern f32 alternating_smoke_burst_vertical_acceleration_decrement_scale_gp __asm__("D_L02_00162120") __attribute__((sda));
extern f32 alternating_smoke_burst_motion_variation_fraction_gp __asm__("D_L02_00162124") __attribute__((sda));
extern f32 alternating_smoke_burst_jitter_magnitude_max_gp __asm__("D_L02_00162128") __attribute__((sda));
extern s32 alternating_smoke_burst_timing_random_span_gp __asm__("D_L02_0016212C") __attribute__((sda));
extern f32 alternating_smoke_burst_timing_variation_fraction_gp __asm__("D_L02_00162138") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L02_GAMEPLAY_ENTITIES_CONSTRUCTORBOT_GLOBALS_H */
