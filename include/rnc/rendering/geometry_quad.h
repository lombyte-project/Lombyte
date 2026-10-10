#ifndef LOMBYTE_RNC_RENDERING_GEOMETRY_QUAD_H
#define LOMBYTE_RNC_RENDERING_GEOMETRY_QUAD_H

#include "types.h"
#include "rnc/math/vector.h"

/* 0x90-byte quad shared by resident and overlay renderers.
   The resident renderer requires ordered writes to the reserved word. */
struct GeometryQuad {
    Vec4 positions[4];            /* 0x00 */
    u32 colors[4];                /* 0x40 */
    f32 texture_coordinates[4][2]; /* 0x50 */
    union {
        u64 value;
        volatile u64 ordered;
    } reserved;                   /* 0x70 */
    u64 texture;                  /* 0x78 */
    u64 texture_state;            /* 0x80 */
    u64 primitive;                /* 0x88 */
};

#endif
