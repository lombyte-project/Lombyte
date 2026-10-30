#ifndef LOMBYTE_RNC_OVERLAY_L13_MOBYS_MINE_H
#define LOMBYTE_RNC_OVERLAY_L13_MOBYS_MINE_H

/* Types and functions of l13/mobys/mine.c that other level files use. */

#include "types.h"

unsigned char *spawn_level_13_mine(char *owner,
                                   char *spawn_position,
                                   char *initial_velocity) __asm__("FUN_L13_002c1f28");

#endif /* LOMBYTE_RNC_OVERLAY_L13_MOBYS_MINE_H */
