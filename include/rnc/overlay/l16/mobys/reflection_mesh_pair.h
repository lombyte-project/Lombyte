#ifndef LOMBYTE_RNC_OVERLAY_L16_MOBYS_REFLECTION_MESH_PAIR_H
#define LOMBYTE_RNC_OVERLAY_L16_MOBYS_REFLECTION_MESH_PAIR_H

#include "types.h"

struct Moby;

void reflection_mesh_pair_update(struct Moby *) __asm__("FUN_L16_002e98e0");
void draw_level16_reflection_mesh_batch(void) __asm__("FUN_L16_002e9ce0");

#endif /* LOMBYTE_RNC_OVERLAY_L16_MOBYS_REFLECTION_MESH_PAIR_H */
