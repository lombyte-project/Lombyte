#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_NANOTECH_BALL_DATA_NANOTECH_BALL_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_NANOTECH_BALL_DATA_NANOTECH_BALL_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/shared/pickups/nanotech_ball.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float nanotech_ball_collection_curve_radius_gp __asm__("D_L00_00161A50")
    __attribute__((sda));
extern float nanotech_ball_collection_target_gravity_offset_gp __asm__("D_L00_00161A54")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_NANOTECH_BALL_DATA_NANOTECH_BALL_UPDATE_H */
