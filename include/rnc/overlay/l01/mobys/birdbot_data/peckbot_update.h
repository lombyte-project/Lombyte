#ifndef LOMBYTE_RNC_OVERLAY_L01_MOBYS_BIRDBOT_DATA_PECKBOT_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_L01_MOBYS_BIRDBOT_DATA_PECKBOT_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/l01/mobys/birdbot.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern f32 peckbot_nonfatal_knockback_vertical_speed_gp __asm__("D_L01_00161A60")
    __attribute__((sda));
extern f32 peckbot_nonfatal_knockback_horizontal_speed_gp __asm__("D_L01_00161A64")
    __attribute__((sda));
extern f32 peckbot_home_knockback_speed_gp __asm__("D_L01_00161A68") __attribute__((sda));
extern f32 peckbot_home_knockback_acceleration_gp __asm__("D_L01_00161A6C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L01_MOBYS_BIRDBOT_DATA_PECKBOT_UPDATE_H */
