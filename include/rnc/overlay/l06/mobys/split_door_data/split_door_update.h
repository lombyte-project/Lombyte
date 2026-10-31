#ifndef LOMBYTE_RNC_OVERLAY_L06_MOBYS_SPLIT_DOOR_DATA_UPDATE_SPLIT_DOOR_H
#define LOMBYTE_RNC_OVERLAY_L06_MOBYS_SPLIT_DOOR_DATA_UPDATE_SPLIT_DOOR_H

#include "types.h"

/* Scalar data for src/overlays/l06/mobys/split_door.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float moby_pair_phase_rotation_y_acceleration_degrees_gp __asm__("D_L06_00161F40")
    __attribute__((sda));
extern float moby_pair_phase_rotation_y_deceleration_degrees_gp __asm__("D_L06_00161F44")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L06_MOBYS_SPLIT_DOOR_DATA_UPDATE_SPLIT_DOOR_H */
