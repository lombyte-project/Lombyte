#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_NANOTECH_BALL_DATA_UPDATE_NANOTECH_BALL_COLLECTION_EFFECTS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_NANOTECH_BALL_DATA_UPDATE_NANOTECH_BALL_COLLECTION_EFFECTS_H

#include "types.h"

/* Scalar data for src/overlays/shared/pickups/nanotech_ball.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern s32 nanotech_ball_collection_second_particle_mode_selector __asm__("D_L00_00161A84")
    __attribute__((sda));
extern s32 nanotech_ball_collection_particle_byte_source_index __asm__("D_L00_00161A88")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_NANOTECH_BALL_DATA_UPDATE_NANOTECH_BALL_COLLECTION_EFFECTS_H */
