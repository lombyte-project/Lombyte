#ifndef LOMBYTE_RNC_OVERLAY_L02_MOBYS_H
#define LOMBYTE_RNC_OVERLAY_L02_MOBYS_H

#include "types.h"

struct Moby;

char *spawn_projectile_moby(struct Moby *, void *, float *) __asm__("FUN_L02_002ec228");
void  update_skid_mcmarx_relocation_sequence(struct Moby *) __asm__("FUN_L02_002e1fb8");

#endif /* LOMBYTE_RNC_OVERLAY_L02_MOBYS_H */
