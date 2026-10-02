#ifndef RNC_GAMEPLAY_CAMERA_FUN_001ED940_TYPES_H
#define RNC_GAMEPLAY_CAMERA_FUN_001ED940_TYPES_H

#include "types.h"

struct CameraControlConditions {
    u8 pad_0[0x2F0];
    f32 height_threshold;
    u8 pad_2F4[0xFF0];
    u8 base_condition;
    u8 selector_1;
    u8 selector_3;
    u8 pad_12E7[0x4];
    u8 selector_11;
    u8 selector_13;
    u8 pad_12ED[0xD97];
    s32 secondary_mode;
    u8 pad_2088[0x4];
    s32 control_mode;
};

struct CameraPosition {
    u8 pad_0[0x8];
    f32 z;
};

struct CameraTrackingControl {
    u8 pad_0[0xC0];
    s32 control_selector;
};

#endif /* RNC_GAMEPLAY_CAMERA_FUN_001ED940_TYPES_H */
