#ifndef LOMBYTE_RNC_MOBYS_ANIMATION_BUILD_MOBY_BONE_TRANSFORM_H
#define LOMBYTE_RNC_MOBYS_ANIMATION_BUILD_MOBY_BONE_TRANSFORM_H

#include "types.h"

struct Moby;

void build_moby_bone_transform(struct Moby *moby,
                               s32          bone_index,
                               s32          output_transform) __asm__("FUN_0020cca8");

#endif /* LOMBYTE_RNC_MOBYS_ANIMATION_BUILD_MOBY_BONE_TRANSFORM_H */
