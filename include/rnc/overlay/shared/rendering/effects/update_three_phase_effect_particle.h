#ifndef LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_UPDATE_THREE_PHASE_EFFECT_PARTICLE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_UPDATE_THREE_PHASE_EFFECT_PARTICLE_H

#include "types.h"

void spawn_effect_particle_with_target_scalar(void *, void *, s32, s32, f32, s32) __asm__("FUN_L00_0026ced0");
void  spawn_color_decaying_effect_particle(void *, void *, s32) __asm__("FUN_L00_0026d598");
void *spawn_source_attached_fading_effect_particle(void *, void *, s32, s32, f32) __asm__("FUN_L00_0026daa0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_UPDATE_THREE_PHASE_EFFECT_PARTICLE_H */
