#ifndef LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_SPAWN_EXPLOSION_H
#define LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_SPAWN_EXPLOSION_H

#include "types.h"
#include "rnc/overlay/shared/world/level_path.h"

void cart_burst(void *owner, void *pos, int sound, float scale, float flash) __asm__("FUN_L00_0025f090");
s32 is_moby_in_class_range_500_to_540(u8 *m) __asm__("FUN_L00_0025e3b8");

s32 FUN_L00_0025d808(LevelPath *, void *, f32 *, s32 *, s32, f32);
f32 normalize_angle(f32) __asm__("FUN_L00_0025e310");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_SPAWN_EXPLOSION_H */
