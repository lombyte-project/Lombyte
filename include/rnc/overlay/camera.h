#ifndef LOMBYTE_RNC_OVERLAY_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_CAMERA_H

#include "types.h"

/* What D_L00_00166E04 points to: the hero-aligned camera crosses the hero's
   up axis with its first vector, and the follow camera setup tests unk86
   against 7. */
typedef struct {
    f32 axis[4];
    u8  pad10[0x76];
    s16 unk86;
} FollowCamSideRef;

#endif /* LOMBYTE_RNC_OVERLAY_CAMERA_H */
