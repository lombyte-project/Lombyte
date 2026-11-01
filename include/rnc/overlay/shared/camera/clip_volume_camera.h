#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_CLIP_VOLUME_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_CLIP_VOLUME_CAMERA_H

#include "types.h"

int  evaluate_clip_volume_candidate(char *, char *) __asm__("FUN_L03_002e8870");
void set_transform_state_three_when_outside_clip_volume(char *) __asm__("FUN_L03_002e8980");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_CLIP_VOLUME_CAMERA_H */
