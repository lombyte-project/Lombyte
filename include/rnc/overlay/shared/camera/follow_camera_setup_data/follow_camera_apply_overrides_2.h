#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_SETUP_DATA_FOLLOW_CAMERA_APPLY_OVERRIDES_2_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_SETUP_DATA_FOLLOW_CAMERA_APPLY_OVERRIDES_2_H

#include "types.h"

/* Scalar data for src/overlays/shared/camera/follow_camera_setup.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern s32 follow_camera_rotation_slide_enabled_gp __asm__("D_L00_00161D94") __attribute__((sda));
extern f32 follow_camera_rotation_slide_trigger_threshold_gp __asm__("D_L00_00161D84")
    __attribute__((sda));
extern f32 follow_camera_rotation_slide_lateral_spring_target_gp __asm__("D_L00_00161D88")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_SETUP_DATA_FOLLOW_CAMERA_APPLY_OVERRIDES_2_H */
