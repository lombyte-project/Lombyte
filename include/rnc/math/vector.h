#ifndef LOMBYTE_RNC_MATH_VECTOR_H
#define LOMBYTE_RNC_MATH_VECTOR_H

#include "types.h"

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

#endif /* LOMBYTE_RNC_MATH_VECTOR_H */
