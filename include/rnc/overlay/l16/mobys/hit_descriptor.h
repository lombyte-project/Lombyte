#ifndef LOMBYTE_RNC_OVERLAY_L16_MOBYS_HIT_DESCRIPTOR_H
#define LOMBYTE_RNC_OVERLAY_L16_MOBYS_HIT_DESCRIPTOR_H

#include "types.h"
#include "rnc/overlay/quad.h"

/* Types rocket.c and test_dummy.c share: a quadword vector they also read as
   floats, and the hit descriptor make_hit_descriptor fills and collision_line
   and collect_mobies_around_tracked_objects take. */
typedef union {
    OvlQuad quad;
    float   f[4];
} L16QuadVector;

typedef struct {
    float          direction[4];
    void          *owner;
    int            flags;
    unsigned char  kind, enabled;
    unsigned short cls;
    float          scale;
    int            active;
} L16HitDescriptor;

#endif /* LOMBYTE_RNC_OVERLAY_L16_MOBYS_HIT_DESCRIPTOR_H */
