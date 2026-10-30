#ifndef LOMBYTE_RNC_OVERLAY_LEVEL_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_LEVEL_CAMERA_H

#include "types.h"
#include "rnc/math/vector.h"

struct Moby;

/* The level's game camera. Turrets and cannons check targets against its
   position, pitch and yaw. A shot sets the two amount/frames pairs (0.03 for
   a few frames), a hard landing sets the first one (0.2 for 40 frames). */
typedef struct {
    u8 pad0[0x140];
    Vec4 pos;                       /* 0x140 */
    f32 unk150;
    f32 pitch;                      /* 0x154 */
    f32 yaw;                        /* 0x158 */
    u8 pad15C[4];
    f32 shake_amount;               /* 0x160 */
    u8 pad164[4];
    s32 shake_frames;               /* 0x168 */
    u8 pad16C[4];
    f32 shake2_amount;              /* 0x170 */
    u8 pad174[4];
    s32 shake2_frames;              /* 0x178 */
    u8 pad17C[0xE0];
    struct Moby *focus_moby;        /* 0x25C: a moby that takes the camera's attention (blarg helicopter); 0: none */
    s32 focus_time;                 /* 0x260: reset to 0 when focus_moby is set */
} LevelCamera;

extern LevelCamera l06_camera __asm__("D_L06_00167400");
extern LevelCamera l08_camera __asm__("D_L08_00167480");
extern LevelCamera l12_camera __asm__("D_L12_00167100");

#endif /* LOMBYTE_RNC_OVERLAY_LEVEL_CAMERA_H */
