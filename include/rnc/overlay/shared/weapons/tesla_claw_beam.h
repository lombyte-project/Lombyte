#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TESLA_CLAW_BEAM_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TESLA_CLAW_BEAM_H

#include "types.h"

typedef struct {
    int   c[4];
    float f[8];
    long  z4, q0, q1, q2;
} RT_2baae0;

struct Moby;

void draw_beam_and_arc_effects(struct Moby *) __asm__("FUN_L00_002ba970");
void build_beam_and_arc_points(struct Moby *) __asm__("FUN_L00_002b94d0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TESLA_CLAW_BEAM_H */
