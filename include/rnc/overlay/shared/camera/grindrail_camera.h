#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_GRINDRAIL_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_GRINDRAIL_CAMERA_H

#include "types.h"

void initialize_camera_transform_and_paths() __asm__("FUN_L01_00315de0");
void update_camera_mode_from_preset() __asm__("FUN_L01_00316030");
void update_camera_position_and_orientation_from_paths() __asm__("FUN_L01_00315358");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_GRINDRAIL_CAMERA_H */
