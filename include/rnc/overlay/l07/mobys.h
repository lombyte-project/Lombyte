#ifndef LOMBYTE_RNC_OVERLAY_L07_MOBYS_H
#define LOMBYTE_RNC_OVERLAY_L07_MOBYS_H

#include "types.h"

int   get_target_sight(char *, char *, float) __asm__("FUN_L07_002886d8");
char *spawn_growing_ballistic_moby(char *, char *, int) __asm__("FUN_L07_0030e370");
unsigned char *
spawn_machinegun_turret_projectile(void *, float *, int, int) __asm__("FUN_L07_0030b538");

#endif /* LOMBYTE_RNC_OVERLAY_L07_MOBYS_H */
