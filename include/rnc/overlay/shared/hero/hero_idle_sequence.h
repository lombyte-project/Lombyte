#ifndef LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_IDLE_SEQUENCE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_IDLE_SEQUENCE_H

#include "types.h"

void release_and_clear_hero_voice_slots(void) __asm__("FUN_L00_00206c08");
void emit_hero_surface_effect_burst(s32 puffs, s32 debris, s32 flash) __asm__("FUN_L00_00209ca8");
s32  return_hero_animation_id_unchanged(s32 input_value) __asm__("FUN_L00_002050b8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_IDLE_SEQUENCE_H */
