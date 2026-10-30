#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_CRATE_DATA_CRATE_DROP_BOLTS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_CRATE_DATA_CRATE_DROP_BOLTS_H

#include "types.h"

/* Scalar data for src/overlays/shared/pickups/crate.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern f32 crate_drop_horizontal_launch_speed_max_gp __asm__("D_L00_001618C8") __attribute__((sda));
extern f32 crate_drop_vertical_launch_speed_min_gp __asm__("D_L00_001618CC") __attribute__((sda));
extern f32 crate_drop_vertical_launch_speed_max_gp __asm__("D_L00_001618D0") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_CRATE_DATA_CRATE_DROP_BOLTS_H */
