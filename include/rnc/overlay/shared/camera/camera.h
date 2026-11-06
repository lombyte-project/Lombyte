#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_CAMERA_H

#include "types.h"

/* What D_L00_00166E04 points to: the hero-aligned camera crosses the hero's
   up axis with its first vector, and the follow camera setup tests its camera_class
   (UpdateCam.camera_class) against 7, the Swingshot camera. */
typedef struct {
    f32 axis[4];
    u8  pad10[0x76];
    s16 camera_class; /* 0x86: UpdateCam.camera_class */
} FollowCamSideRef;

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_CAMERA_H */
