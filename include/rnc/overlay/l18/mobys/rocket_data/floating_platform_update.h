#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_ROCKET_DATA_FLOATING_PLATFORM_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_ROCKET_DATA_FLOATING_PLATFORM_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/l18/mobys/rocket.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float floating_platform_motion_scale_input_start_gp __asm__("D_L18_00161A60")
    __attribute__((sda));
extern float floating_platform_motion_scale_input_end_gp __asm__("D_L18_00161A64")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_ROCKET_DATA_FLOATING_PLATFORM_UPDATE_H */
