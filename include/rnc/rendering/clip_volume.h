#ifndef LOMBYTE_RNC_RENDERING_CLIP_VOLUME_H
#define LOMBYTE_RNC_RENDERING_CLIP_VOLUME_H

#include "types.h"

/* Tests a point against a clip volume. */
s32 is_point_inside_clip_volume(void *point, s32 volume_index) __asm__("FUN_00214720");

#endif /* LOMBYTE_RNC_RENDERING_CLIP_VOLUME_H */
