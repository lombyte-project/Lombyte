#ifndef LOMBYTE_RNC_OVERLAY_L03_MOBYS_TAXI_H
#define LOMBYTE_RNC_OVERLAY_L03_MOBYS_TAXI_H

/* The taxi rail path step, defined in src/overlays/l03/mobys/taxi.c; rail_lift.c moves along its rail paths with it too. */
#include "types.h"

s32 advance_moby_along_rail_path(struct Moby *rail_moby,
                                 s32          path,
                                 s32         *snd) __asm__("FUN_L03_002d3918");

#endif /* LOMBYTE_RNC_OVERLAY_L03_MOBYS_TAXI_H */
