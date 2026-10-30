#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_SETUP_DATA_FOLLOW_CAMERA_APPLY_OVERRIDES_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_SETUP_DATA_FOLLOW_CAMERA_APPLY_OVERRIDES_H

#include "types.h"

/* Scalar data for src/overlays/shared/camera/follow_camera_setup.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern f32 follow_camera_vehicle_collision_probe_step_growth_factor_gp __asm__("D_L00_00161DA4")
    __attribute__((sda));
extern f32 follow_camera_vehicle_collision_probe_scale_gp __asm__("D_L00_00161DA8")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_SETUP_DATA_FOLLOW_CAMERA_APPLY_OVERRIDES_H */
