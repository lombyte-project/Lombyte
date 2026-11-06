#ifndef LOMBYTE_RNC_OVERLAY_L05_H
#define LOMBYTE_RNC_OVERLAY_L05_H

#include "types.h"

struct Moby;

/* The level's moby instances, 0x100 bytes each; mobys refer to one another by index into it. */
extern struct Moby *l05_moby_array __asm__("D_L05_0015FFD8");

void render_level_water_grids(void) __asm__("FUN_L05_0030d678");

#endif /* LOMBYTE_RNC_OVERLAY_L05_H */
