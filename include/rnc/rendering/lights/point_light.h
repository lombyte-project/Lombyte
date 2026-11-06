#ifndef LOMBYTE_RNC_RENDERING_LIGHTS_POINT_LIGHT_H
#define LOMBYTE_RNC_RENDERING_LIGHTS_POINT_LIGHT_H

#include "types.h"

/* One of the 8 point lights at D_0019C1C0 (0x20 bytes); the level code's
   PointLight has the same layout. */
struct PointLight {
    f32 color[3];
    f32 intensity;
    f32 sphere[4]; /* 0x10: position, w: radius */
};

/* Link bookkeeping for one point light at D_0019C3C0 (0x30 bytes): the
   start/count pairs of its entries in instance_indices, in tie, shrub,
   tfrag order (docs/game/point-lights.md). */
struct PointLightSlot {
    s16  tie_start;
    s16  tie_count;
    s16  shrub_start;
    s16  shrub_count;
    s16  tfrag_start;
    s16  tfrag_count;
    s16 *instance_indices; /* 0x0C */
    s32  link_state;       /* 0x10: 0 off, 1 links to create, 2 linked */
    u8   pad_14[0xC];
    f32  linked_sphere[4]; /* 0x20: the light's sphere when its links were built */
};

#endif /* LOMBYTE_RNC_RENDERING_LIGHTS_POINT_LIGHT_H */
