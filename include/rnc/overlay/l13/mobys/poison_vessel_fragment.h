#ifndef LOMBYTE_RNC_OVERLAY_L13_MOBYS_POISON_VESSEL_FRAGMENT_H
#define LOMBYTE_RNC_OVERLAY_L13_MOBYS_POISON_VESSEL_FRAGMENT_H

/* Types and functions of l13/mobys/poison_vessel_fragment.c that other level files use. */

#include "types.h"

char *spawn_poison_vessel_fragment(void *position,
                                   void *initial_velocity,
                                   int   duration_frames,
                                   int   effect_mode) __asm__("FUN_L13_0030c138");

#endif /* LOMBYTE_RNC_OVERLAY_L13_MOBYS_POISON_VESSEL_FRAGMENT_H */
