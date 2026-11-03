#ifndef LOMBYTE_RNC_MOBYS_ANIMATION_H
#define LOMBYTE_RNC_MOBYS_ANIMATION_H

#include "types.h"

struct InterpolatedAnim;

s32 is_value_within_interpolated_window(struct InterpolatedAnim *, f32) __asm__("FUN_00214cc8");
/* cos and sin of k * 2pi / 256; process_moby_anim_data copies it whole (0x800 bytes)
   into scratchpad for the moby and tfrag lighting code. */
extern f32 normal_lut[256][2] __asm__("D_00165500");

s32 queue_animation_update(s32, s32, s32, s32, s32, s32, s32) __asm__("FUN_001ff308");

#endif /* LOMBYTE_RNC_MOBYS_ANIMATION_H */
