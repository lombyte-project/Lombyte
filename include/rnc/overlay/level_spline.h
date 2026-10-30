#ifndef LOMBYTE_RNC_OVERLAY_LEVEL_SPLINE_H
#define LOMBYTE_RNC_OVERLAY_LEVEL_SPLINE_H

#include "types.h"
#include "rnc/overlay/level_path.h"

/* A 0x20-byte entry of the level's spline table. Racers ride path, and
   hero.rail_spline holds the path of the rail the hero grinds. The level
   loader copies bounds and kind from the level file and points path into
   the path data it copies after the table. */
typedef struct {
    Vec4       bounds; /* xyz centre, w radius: rail checks skip a spline farther away */
    LevelPath *path;   /* 0x10 */
    s32        kind;   /* 0x14 */
    u8         pad18[8];
} LevelSpline;

/* Each level's spline table; the loader sets it, D_L00_0015F710 holds the count. */
extern LevelSpline *l00_splines __asm__("D_L00_0015F70C");
extern LevelSpline *l01_splines __asm__("D_L01_0015F70C");
extern LevelSpline *l03_splines __asm__("D_L03_0015F70C");
extern LevelSpline *l14_splines __asm__("D_L14_0015F70C");
extern LevelSpline *l18_splines __asm__("D_L18_0015F70C");

#endif /* LOMBYTE_RNC_OVERLAY_LEVEL_SPLINE_H */
