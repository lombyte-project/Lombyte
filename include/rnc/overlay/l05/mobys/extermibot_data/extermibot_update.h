#ifndef LOMBYTE_RNC_OVERLAY_L05_MOBYS_EXTERMIBOT_DATA_EXTERMIBOT_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_L05_MOBYS_EXTERMIBOT_DATA_EXTERMIBOT_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/l05/mobys/extermibot.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float extermibot_spark_facing_xy_scale_gp __asm__("D_L05_00161AB8") __attribute__((sda));
extern float extermibot_spark_random_z_scale_gp __asm__("D_L05_00161ABC") __attribute__((sda));
extern float extermibot_spark_facing_vector_w_gp __asm__("D_L05_00161AC0") __attribute__((sda));
extern float extermibot_spark_random_vector_w_gp __asm__("D_L05_00161AC4") __attribute__((sda));
extern int   extermibot_pre_strike_delay_frames_gp __asm__("D_L05_00161AC8") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L05_MOBYS_EXTERMIBOT_DATA_EXTERMIBOT_UPDATE_H */
