#ifndef LOMBYTE_RNC_RENDERING_EFFECTS_H
#define LOMBYTE_RNC_RENDERING_EFFECTS_H

#include "types.h"

struct TransitionState;

void append_billboard_batch(void) __asm__("FUN_001f92b0");
void draw_resident_textured_banner(s32) __asm__("FUN_0022ea08");
s32  draw_transition_overlay(struct TransitionState *) __asm__("FUN_0021fc68");

#endif /* LOMBYTE_RNC_RENDERING_EFFECTS_H */
