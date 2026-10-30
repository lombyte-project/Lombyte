#ifndef LOMBYTE_RNC_OVERLAY_L13_EFFECT_SUPPRESSION_LEVELS_H
#define LOMBYTE_RNC_OVERLAY_L13_EFFECT_SUPPRESSION_LEVELS_H

#include "types.h"

/* The level's effect suppression levels: classes skip optional glows and
   fire effects while these are high (blarg_space_fighter.c, forcefield_tower.c). */
extern float optional_glow_suppression_level __asm__("D_L13_0015F5D0");
extern float blarg_space_fighter_fire_suppression_level __asm__("D_L13_0015F5D4");

#endif /* LOMBYTE_RNC_OVERLAY_L13_EFFECT_SUPPRESSION_LEVELS_H */
