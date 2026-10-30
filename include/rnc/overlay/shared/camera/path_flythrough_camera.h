#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_PATH_FLYTHROUGH_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_PATH_FLYTHROUGH_CAMERA_H

#include "types.h"

struct UpdateCam;

/* Camera type 19, the path flythrough camera (src/overlays/shared/camera/path_flythrough_camera.c). */
void activate_descriptor(int i) __asm__("FUN_L10_002f5a50");
int  check_descriptor_hero_trigger(char *moby) __asm__("FUN_L10_002f5a78");
void initialize_path_camera(struct UpdateCam *path_camera) __asm__("FUN_L10_002f5b58");
int  evaluate_descriptor_eligibility(char *descriptor_owner,
                                     char *comparison_object) __asm__("FUN_L10_002f65f8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_PATH_FLYTHROUGH_CAMERA_H */
