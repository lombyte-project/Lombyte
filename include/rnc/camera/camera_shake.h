#ifndef LOMBYTE_RNC_CAMERA_CAMERA_SHAKE_H
#define LOMBYTE_RNC_CAMERA_CAMERA_SHAKE_H

#include "types.h"

/* One camera shake channel. apply_camera_shake counts frames down and moves
   the camera along a view row by amplitude, fading out over frames_max. */
struct CameraShake {
    f32 amplitude;  /* 0x0 */
    f32 offset;     /* 0x4: this frame's move */
    s32 frames;     /* 0x8: frames left */
    s32 frames_max; /* 0xC: the longest count since the shake started */
};

#endif /* LOMBYTE_RNC_CAMERA_CAMERA_SHAKE_H */
