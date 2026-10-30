#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_DEVASTATOR_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_DEVASTATOR_H

#include "types.h"

typedef unsigned int Quad_2b2100 __attribute__((mode(TI)));

typedef union {
    struct {
        float x, y, z, w;
    } f;
    Quad_2b2100 q;
} Vec_2b2100 __attribute__((aligned(16)));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_DEVASTATOR_H */
