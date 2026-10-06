#ifndef LOMBYTE_RNC_MATH_VECTOR_H
#define LOMBYTE_RNC_MATH_VECTOR_H

#include "types.h"
#include "eetypes.h"

/* Plain float vectors with 4-byte alignment (no u128 member). */
typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4f;

/* A 16-byte aligned quadword vector, read as a whole (q), as floats or
   as words. */
typedef union {
    u128 q;
    f32 f[4];
    s32 i[4];
} Vec4;

#endif /* LOMBYTE_RNC_MATH_VECTOR_H */
