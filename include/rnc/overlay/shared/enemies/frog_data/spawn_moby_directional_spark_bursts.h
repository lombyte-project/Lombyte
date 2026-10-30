#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_FROG_DATA_SPAWN_MOBY_DIRECTIONAL_SPARK_BURSTS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_FROG_DATA_SPAWN_MOBY_DIRECTIONAL_SPARK_BURSTS_H

#include "types.h"

/* Scalar data for src/overlays/shared/enemies/frog.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern s32 moby_spark_first_color_range_start_gp __asm__("D_L06_00161C48") __attribute__((sda));
extern s32 moby_spark_second_color_range_start_gp __asm__("D_L06_00161C4C") __attribute__((sda));
extern s32 moby_spark_first_color_range_end_gp __asm__("D_L06_00161C50") __attribute__((sda));
extern s32 moby_spark_second_color_range_end_gp __asm__("D_L06_00161C54") __attribute__((sda));
extern s32 moby_spark_first_frame_count_base_gp __asm__("D_L06_00161C58") __attribute__((sda));
extern s32 moby_spark_second_frame_count_base_gp __asm__("D_L06_00161C5C") __attribute__((sda));
extern s32 moby_spark_third_frame_count_base_gp __asm__("D_L06_00161C60") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_FROG_DATA_SPAWN_MOBY_DIRECTIONAL_SPARK_BURSTS_H */
