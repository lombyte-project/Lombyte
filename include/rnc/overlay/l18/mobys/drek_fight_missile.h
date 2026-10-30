#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_DREK_FIGHT_MISSILE_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_DREK_FIGHT_MISSILE_H

#include "types.h"

/* Declarations drek_fight_missile.c shares with other files of its level. */
int activate_drek_fight_missiles_in_group(
    char *moby, int group, float *missile_activation_position) __asm__("FUN_L18_002f0030");

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_DREK_FIGHT_MISSILE_H */
