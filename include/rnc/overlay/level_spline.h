#ifndef LOMBYTE_RNC_OVERLAY_LEVEL_SPLINE_H
#define LOMBYTE_RNC_OVERLAY_LEVEL_SPLINE_H

#include "types.h"
#include "rnc/overlay/level_path.h"

/* A 0x20-byte entry of the level's spline table. Racers ride path, and
   hero.rail_spline holds the path of the rail the hero grinds. */
typedef struct {
    u8 pad0[0x10];
    LevelPath *path;                /* 0x10 */
    u8 pad14[0xC];
} LevelSpline;

extern LevelSpline *l14_splines __asm__("D_L14_0015F70C");

#endif /* LOMBYTE_RNC_OVERLAY_LEVEL_SPLINE_H */
