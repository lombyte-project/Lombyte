#ifndef LOMBYTE_RNC_OVERLAY_L02_MOBYS_SMOKE_VENT_DATA_UPDATE_SMOKE_VENT_6_H
#define LOMBYTE_RNC_OVERLAY_L02_MOBYS_SMOKE_VENT_DATA_UPDATE_SMOKE_VENT_6_H

#include "types.h"

/* Scalar data for src/overlays/l02/mobys/smoke_vent.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern f32 view_gated_smoke_puff_velocity_magnitude_scale_gp __asm__("D_L02_00162088")
    __attribute__((sda));
extern f32 view_gated_smoke_puff_vertical_acceleration_scale_gp __asm__("D_L02_0016208C")
    __attribute__((sda));
extern f32 view_gated_smoke_puff_lateral_acceleration_magnitude_bound_gp __asm__("D_L02_00162090")
    __attribute__((sda));
extern f32 view_gated_smoke_puff_motion_variation_fraction_gp __asm__("D_L02_00162094")
    __attribute__((sda));
extern s32 view_gated_smoke_puff_timing_random_span_gp __asm__("D_L02_00162098")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L02_MOBYS_SMOKE_VENT_DATA_UPDATE_SMOKE_VENT_6_H */
