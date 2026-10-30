#ifndef LOMBYTE_RNC_OVERLAY_L11_MOBYS_SEESAW_PLATFORM_DATA_PUFFOID_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_L11_MOBYS_SEESAW_PLATFORM_DATA_PUFFOID_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/l11/mobys/seesaw_platform.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float puffoid_spray_target_distance_metric_threshold_gp __asm__("D_L11_001622E4")
    __attribute__((sda));
extern float puffoid_chase_heading_offset_limit_degrees_gp __asm__("D_L11_001622E8")
    __attribute__((sda));
extern float puffoid_jump_horizontal_speed_gp __asm__("D_L11_001622EC") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L11_MOBYS_SEESAW_PLATFORM_DATA_PUFFOID_UPDATE_H */
