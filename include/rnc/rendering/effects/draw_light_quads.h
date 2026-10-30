#ifndef LOMBYTE_RNC_RENDERING_EFFECTS_DRAW_LIGHT_QUADS_H
#define LOMBYTE_RNC_RENDERING_EFFECTS_DRAW_LIGHT_QUADS_H

#include "types.h"

typedef float FVec4[4] __attribute__((aligned(16)));

typedef struct {
    FVec4 v;
} FRow;

/* The 4 corners of a light quad: x, y offsets and u, v (draw_light_quads
   loops over 4). */
extern FRow light_quad_vertex_template[4] __asm__("D_0018CAA0");

#endif /* LOMBYTE_RNC_RENDERING_EFFECTS_DRAW_LIGHT_QUADS_H */
