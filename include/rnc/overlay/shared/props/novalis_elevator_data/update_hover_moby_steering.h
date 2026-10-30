#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PROPS_NOVALIS_ELEVATOR_DATA_UPDATE_HOVER_MOBY_STEERING_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PROPS_NOVALIS_ELEVATOR_DATA_UPDATE_HOVER_MOBY_STEERING_H

#include "types.h"

/* Scalar data for src/overlays/shared/props/novalis_elevator.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern f32 hover_target_yaw_spring_strength_degrees_gp __asm__("D_L01_00161944")
    __attribute__((sda));
extern f32 hover_target_yaw_damping_degrees_gp __asm__("D_L01_00161948") __attribute__((sda));
extern f32 hover_target_yaw_turn_rate_limit_degrees_gp __asm__("D_L01_0016194C")
    __attribute__((sda));
extern f32 hover_ground_animation_height_threshold_gp __asm__("D_L01_00161920")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PROPS_NOVALIS_ELEVATOR_DATA_UPDATE_HOVER_MOBY_STEERING_H */
