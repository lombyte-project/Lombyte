#ifndef LOMBYTE_RNC_OVERLAY_QUAD_H
#define LOMBYTE_RNC_OVERLAY_QUAD_H

#include "types.h"

/* 128-bit value, for whole-quadword copies. Overlay files define their own
   u128, so they use this instead of eetypes.h. */
typedef int OvlQuad __attribute__((mode(TI)));

/* Same layout as Vec4 (rnc/math/vector.h), built on OvlQuad. */
typedef union {
    OvlQuad q;
    f32 f[4];
    s32 i[4];
} OvlVec4;

#endif /* LOMBYTE_RNC_OVERLAY_QUAD_H */
