#ifndef LOMBYTE_RNC_OVERLAY_L12_MOBYS_WATER_RESERVOIR_DATA_WATER_RESERVOIR_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_L12_MOBYS_WATER_RESERVOIR_DATA_WATER_RESERVOIR_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/l12/mobys/water_reservoir.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float water_reservoir_unmirrored_effect_spawn_z_offset_gp __asm__("D_L12_001619FC")
    __attribute__((sda));
extern float water_reservoir_effect_delta_z_reference_gp __asm__("D_L12_00161A00")
    __attribute__((sda));
extern float water_reservoir_mirrored_effect_spawn_z_offset_gp __asm__("D_L12_00161A04")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L12_MOBYS_WATER_RESERVOIR_DATA_WATER_RESERVOIR_UPDATE_H */
