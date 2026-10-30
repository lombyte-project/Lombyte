#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_LARGE_AMOEBOID_DATA_SPAWN_AMOEBOID_BURST_SPARKS_2_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_LARGE_AMOEBOID_DATA_SPAWN_AMOEBOID_BURST_SPARKS_2_H

#include "types.h"

/* Scalar data for src/overlays/shared/enemies/large_amoeboid.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern s32 amoeboid_burst_spark_spawn_color_randomization_start_gp __asm__("D_L01_00161A1C")
    __attribute__((sda));
extern s32 amoeboid_burst_spark_secondary_color_randomization_start_gp __asm__("D_L01_00161A20")
    __attribute__((sda));
extern s32 amoeboid_burst_spark_spawn_color_randomization_end_gp __asm__("D_L01_00161A24")
    __attribute__((sda));
extern s32 amoeboid_burst_spark_secondary_color_randomization_end_gp __asm__("D_L01_00161A28")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_LARGE_AMOEBOID_DATA_SPAWN_AMOEBOID_BURST_SPARKS_2_H */
