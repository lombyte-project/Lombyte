#ifndef LOMBYTE_RNC_OVERLAY_L01_MOBYS_DEBRIS_CLASS_CHOICES_H
#define LOMBYTE_RNC_OVERLAY_L01_MOBYS_DEBRIS_CLASS_CHOICES_H

#include "types.h"

/* The three debris chunk classes a breaking rock picks from at random
   (rock_formation.c, destroyable_cave_wall.c). */
typedef struct {
    u32 oclass[3];
} DebrisClassChoices;

#endif /* LOMBYTE_RNC_OVERLAY_L01_MOBYS_DEBRIS_CLASS_CHOICES_H */
