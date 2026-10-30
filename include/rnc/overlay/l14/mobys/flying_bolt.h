#ifndef LOMBYTE_RNC_OVERLAY_L14_MOBYS_FLYING_BOLT_H
#define LOMBYTE_RNC_OVERLAY_L14_MOBYS_FLYING_BOLT_H

/* Types and functions of l14/mobys/flying_bolt.c that other level files use. */

#include "types.h"

char *spawn_flying_bolt(void  *spawn_position,
                        float *launch_vector,
                        int    collision_ignore_moby,
                        int    collision_mode) __asm__("FUN_L14_002eccf0");

#endif /* LOMBYTE_RNC_OVERLAY_L14_MOBYS_FLYING_BOLT_H */
