#ifndef LOMBYTE_RNC_GAMEPLAY_CAMERA_REFRESH_CAMERA_CONTROL_FLAGS_H
#define LOMBYTE_RNC_GAMEPLAY_CAMERA_REFRESH_CAMERA_CONTROL_FLAGS_H

#include "types.h"

struct CameraPosition {
    u8 pad_0[0x8];
    f32 z;
};

struct CameraTrackingControl {
    u8 pad_0[0xC0];
    s32 control_selector;
};

#endif /* LOMBYTE_RNC_GAMEPLAY_CAMERA_REFRESH_CAMERA_CONTROL_FLAGS_H */
