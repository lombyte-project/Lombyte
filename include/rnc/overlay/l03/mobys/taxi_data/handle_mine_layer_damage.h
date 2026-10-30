#ifndef LOMBYTE_RNC_OVERLAY_L03_MOBYS_TAXI_DATA_HANDLE_MINE_LAYER_DAMAGE_H
#define LOMBYTE_RNC_OVERLAY_L03_MOBYS_TAXI_DATA_HANDLE_MINE_LAYER_DAMAGE_H

#include "types.h"

/* Scalar data for src/overlays/l03/mobys/taxi.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern f32 mine_layer_nonlethal_knockback_horizontal_speed_scale_gp __asm__("D_L03_00161900")
    __attribute__((sda));
extern f32 mine_layer_nonlethal_knockback_vertical_launch_input_gp __asm__("D_L03_00161904")
    __attribute__((sda));
extern f32 mine_layer_lethal_knockback_horizontal_speed_scale_gp __asm__("D_L03_00161908")
    __attribute__((sda));
extern f32 mine_layer_lethal_knockback_vertical_launch_input_gp __asm__("D_L03_0016190C")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L03_MOBYS_TAXI_DATA_HANDLE_MINE_LAYER_DAMAGE_H */
