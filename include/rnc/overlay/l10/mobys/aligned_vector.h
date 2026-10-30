#ifndef LOMBYTE_RNC_OVERLAY_L10_MOBYS_ALIGNED_VECTOR_H
#define LOMBYTE_RNC_OVERLAY_L10_MOBYS_ALIGNED_VECTOR_H

#include "types.h"

/* Level 10 enemies (screamer, darter, toxic crab, blarg ship): the x, y, z, w float vector of their
   state blocks and locals. It is Vec4f with quadword alignment, so qcopy can move it whole. */
typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) AlignedVec4f;

#endif /* LOMBYTE_RNC_OVERLAY_L10_MOBYS_ALIGNED_VECTOR_H */
