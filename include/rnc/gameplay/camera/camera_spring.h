#ifndef LOMBYTE_RNC_GAMEPLAY_CAMERA_CAMERA_SPRING_H
#define LOMBYTE_RNC_GAMEPLAY_CAMERA_CAMERA_SPRING_H

#include "types.h"

typedef struct {
    f32 dist;           /* 0x00 */
    f32 unk4;           /* 0x04 */
    u8 pad8[8];
    f32 height;         /* 0x10 */
    u8 pad14[0xC];
} CameraSpring;

#endif /* LOMBYTE_RNC_GAMEPLAY_CAMERA_CAMERA_SPRING_H */
