#ifndef LOMBYTE_RNC_OVERLAY_L08_MOBYS_FALLING_DEBRIS_DATA_UPDATE_FALLING_DEBRIS_H
#define LOMBYTE_RNC_OVERLAY_L08_MOBYS_FALLING_DEBRIS_DATA_UPDATE_FALLING_DEBRIS_H

#include "types.h"

/* Scalar data for src/overlays/l08/mobys/falling_debris.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float falling_debris_burst_primary_vertical_scale __asm__("D_L08_00161C0C")
    __attribute__((sda));
extern float falling_debris_burst_secondary_vertical_scale __asm__("D_L08_00161C10")
    __attribute__((sda));
extern float falling_debris_burst_primary_planar_scale __asm__("D_L08_00161C14")
    __attribute__((sda));
extern float falling_debris_burst_secondary_planar_scale __asm__("D_L08_00161C18")
    __attribute__((sda));
extern int falling_debris_burst_frame_count_random_min __asm__("D_L08_00161C1C")
    __attribute__((sda));
extern int falling_debris_burst_secondary_frame_count_random_min __asm__("D_L08_00161C20")
    __attribute__((sda));
extern int falling_debris_burst_tertiary_frame_count_random_min __asm__("D_L08_00161C24")
    __attribute__((sda));
extern float falling_debris_burst_primary_vector_w __asm__("D_L08_00161C28") __attribute__((sda));
extern float falling_debris_burst_secondary_vector_w __asm__("D_L08_00161C2C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L08_MOBYS_FALLING_DEBRIS_DATA_UPDATE_FALLING_DEBRIS_H */
