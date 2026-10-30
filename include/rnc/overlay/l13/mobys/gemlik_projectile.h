#ifndef LOMBYTE_RNC_OVERLAY_L13_MOBYS_GEMLIK_PROJECTILE_H
#define LOMBYTE_RNC_OVERLAY_L13_MOBYS_GEMLIK_PROJECTILE_H

/* Types and functions of l13/mobys/gemlik_projectile.c that other level files use. */

#include "types.h"

char *spawn_gemlik_projectile(float  speed,
                              float *spawn_position,
                              float *launch_displacement,
                              void  *source_moby) __asm__("FUN_L13_003061c8");

#endif /* LOMBYTE_RNC_OVERLAY_L13_MOBYS_GEMLIK_PROJECTILE_H */
