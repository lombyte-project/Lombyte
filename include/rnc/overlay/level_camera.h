#ifndef LOMBYTE_RNC_OVERLAY_LEVEL_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_LEVEL_CAMERA_H

#include "types.h"
#include "rnc/math/vector.h"

/* The fields of the level's player camera that level code reads. */
typedef struct {
    u8 pad0[0x140];
    Vec4f pos;          /* 0x140 */
    Vec4f rot;          /* 0x150: z is the yaw */
    f32 shake;          /* 0x160 */
    u8 pad164[4];
    s32 shake_time;     /* 0x168 */
} LevelCamera;

extern LevelCamera l06_camera __asm__("D_L06_00167400");

#endif /* LOMBYTE_RNC_OVERLAY_LEVEL_CAMERA_H */
