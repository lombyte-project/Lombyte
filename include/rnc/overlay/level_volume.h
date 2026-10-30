#ifndef LOMBYTE_RNC_OVERLAY_LEVEL_VOLUME_H
#define LOMBYTE_RNC_OVERLAY_LEVEL_VOLUME_H

#include "types.h"
#include "rnc/math/vector.h"

/* A 0x80-byte volume of the level's volume table: a box around origin. An
   offset from origin taken through inv_axis lies in [-1, 1] on each axis
   inside the box (the test is_point_inside_clip_volume makes). Camera spots
   pass origin and rotation to set_race_camera. */
typedef struct {
    Vec4 axis[3];
    Vec4 origin;                    /* 0x30 */
    Vec4 inv_axis[3];               /* 0x40 */
    Vec4 rotation;                  /* 0x70: euler angles */
} LevelVolume;

extern LevelVolume *l10_volumes __asm__("D_L10_001600EC");

#endif /* LOMBYTE_RNC_OVERLAY_LEVEL_VOLUME_H */
