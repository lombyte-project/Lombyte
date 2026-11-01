#ifndef LOMBYTE_RNC_MOBYS_ANIMATION_H
#define LOMBYTE_RNC_MOBYS_ANIMATION_H

#include "types.h"

struct InterpolatedAnim;

s32 is_value_within_interpolated_window(struct InterpolatedAnim *, f32) __asm__("FUN_00214cc8");
s32 queue_animation_update(s32, s32, s32, s32, s32, s32, s32) __asm__("FUN_001ff308");

#endif /* LOMBYTE_RNC_MOBYS_ANIMATION_H */
