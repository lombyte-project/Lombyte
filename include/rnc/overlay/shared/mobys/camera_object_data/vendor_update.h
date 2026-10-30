#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_CAMERA_OBJECT_DATA_VENDOR_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_CAMERA_OBJECT_DATA_VENDOR_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/shared/mobys/camera_object.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern f32 vendor_color_pulse_phase __asm__("D_L00_001613A4") __attribute__((sda));
extern f32 vendor_manipulator_rotation_phase __asm__("D_L00_0016139C") __attribute__((sda));
extern f32 vendor_manipulator_counter_rotation_phase __asm__("D_L00_001613A0") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_CAMERA_OBJECT_DATA_VENDOR_UPDATE_H */
