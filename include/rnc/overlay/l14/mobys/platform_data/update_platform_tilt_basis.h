#ifndef LOMBYTE_RNC_OVERLAY_L14_MOBYS_PLATFORM_DATA_UPDATE_PLATFORM_TILT_BASIS_H
#define LOMBYTE_RNC_OVERLAY_L14_MOBYS_PLATFORM_DATA_UPDATE_PLATFORM_TILT_BASIS_H

#include "types.h"

/* Scalar data for src/overlays/l14/mobys/platform.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float platform_max_tilt_degrees __asm__("D_L14_00161B00") __attribute__((sda));
extern float platform_tilt_direction_z_offset __asm__("D_L14_00161B04") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L14_MOBYS_PLATFORM_DATA_UPDATE_PLATFORM_TILT_BASIS_H */
