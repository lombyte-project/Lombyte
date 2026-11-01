#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_SWING_VOLUME_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_SWING_VOLUME_CAMERA_H

#include "types.h"

typedef struct {
    f32   pos[3]; /* 0x00: aim cameras look from here */
    u8    pad0C[0xC];
    f32   yaw;    /* 0x18: swing cameras turn around this */
    void *params; /* 0x1C: SwingCameraParams or AimCameraParams */
} CameraDescriptor;

struct UpdateCam;

s32 volume_camera_swing_check(struct UpdateCam *) __asm__("FUN_L00_002ed230");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_SWING_VOLUME_CAMERA_H */
