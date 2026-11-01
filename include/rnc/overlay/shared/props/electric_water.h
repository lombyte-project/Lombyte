#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PROPS_ELECTRIC_WATER_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PROPS_ELECTRIC_WATER_H

#include "types.h"

struct Moby;

void electric_water_update(struct Moby *) __asm__("FUN_L15_002d6810");
void draw_electric_water_bolt_effect(char *) __asm__("FUN_L15_002d8710");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PROPS_ELECTRIC_WATER_H */
