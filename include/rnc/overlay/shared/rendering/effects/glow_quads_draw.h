#ifndef LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_GLOW_QUADS_DRAW_H
#define LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_GLOW_QUADS_DRAW_H

#include "types.h"

void release_point_light_slot(s32) __asm__("FUN_L00_0023e838");

void glow_quads_draw(char *) __asm__("FUN_L00_0023db30");
void emit_moby_particle_pairs(void *) __asm__("FUN_L00_0023e008");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_GLOW_QUADS_DRAW_H */
