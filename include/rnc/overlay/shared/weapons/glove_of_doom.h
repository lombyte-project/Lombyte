#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_GLOVE_OF_DOOM_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_GLOVE_OF_DOOM_H

#include "types.h"

struct Moby;

void glove_of_doom_update(struct Moby *) __asm__("FUN_L00_002c7a58");
void spawn_moby_collision_particles(unsigned char *) __asm__("FUN_L00_002c8078");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_GLOVE_OF_DOOM_H */
