#ifndef LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_BEAM_SLOTS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_BEAM_SLOTS_H

#include "types.h"

s32 register_beam_slot(void *, s32, s32) __asm__("FUN_L00_002e1f28");
s32 set_beam_origin_transform_and_endpoint(void *, void *, void *) __asm__("FUN_L00_002e2190");

void release_beam_slot(int) __asm__("FUN_L00_002e20f8");
void update_slot_panes_and_effects(int, int) __asm__("FUN_L00_002e2250");
void draw_active_beam_slots(char *) __asm__("FUN_L00_002e2af0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_BEAM_SLOTS_H */
