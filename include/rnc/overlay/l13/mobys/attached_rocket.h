#ifndef LOMBYTE_RNC_OVERLAY_L13_MOBYS_ATTACHED_ROCKET_H
#define LOMBYTE_RNC_OVERLAY_L13_MOBYS_ATTACHED_ROCKET_H

/* Types and functions of l13/mobys/attached_rocket.c that other level files use. */

#include "types.h"

char *create_attached_rocket(struct Moby *source_moby,
                             short        joint_index) __asm__("FUN_L13_003058a8");

void launch_attached_rocket(struct Moby *moby,
                            char        *pos,
                            int          distance_reference_moby,
                            float        max_travel_distance) __asm__("FUN_L13_00305988");

#endif /* LOMBYTE_RNC_OVERLAY_L13_MOBYS_ATTACHED_ROCKET_H */
