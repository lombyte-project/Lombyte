#ifndef LOMBYTE_RNC_OVERLAY_ENTITIES_H
#define LOMBYTE_RNC_OVERLAY_ENTITIES_H

#include "types.h"

typedef struct {
    char v[16];
    char padv[16];
    float f20;
    char pad24[4];
    float f28;
    char pad2C[4];
    int f30;
    int f34;
} Child;

typedef struct {
    int a, b;
} Pair;

#endif /* LOMBYTE_RNC_OVERLAY_ENTITIES_H */
