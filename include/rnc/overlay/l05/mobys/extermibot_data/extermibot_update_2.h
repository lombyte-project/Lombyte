#ifndef LOMBYTE_RNC_OVERLAY_L05_MOBYS_EXTERMIBOT_DATA_EXTERMIBOT_UPDATE_2_H
#define LOMBYTE_RNC_OVERLAY_L05_MOBYS_EXTERMIBOT_DATA_EXTERMIBOT_UPDATE_2_H

#include "types.h"

/* Scalar data for src/overlays/l05/mobys/extermibot.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int extermibot_primary_spark_initial_color_gp __asm__("D_L05_00161A9C") __attribute__((sda));
extern int extermibot_primary_spark_paired_color_gp __asm__("D_L05_00161AA0") __attribute__((sda));
extern int extermibot_secondary_spark_initial_color_gp __asm__("D_L05_00161AA4")
    __attribute__((sda));
extern int extermibot_secondary_spark_paired_color_gp __asm__("D_L05_00161AA8")
    __attribute__((sda));
extern float extermibot_spark_lifetime_min_frames_gp __asm__("D_L05_00161AAC") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L05_MOBYS_EXTERMIBOT_DATA_EXTERMIBOT_UPDATE_2_H */
