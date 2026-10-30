#ifndef LOMBYTE_RNC_MOBYS_MOBY_SPAWN_H
#define LOMBYTE_RNC_MOBYS_MOBY_SPAWN_H

#include "types.h"

struct Moby;

/* Spawns a moby of class oclass. */
struct Moby *create_moby(s32 oclass) __asm__("FUN_0020c4f8");
/* Marks the moby for removal (state 0xFD or 0xFE). */
void mark_moby_for_removal(struct Moby *moby) __asm__("FUN_0020c828");

#endif /* LOMBYTE_RNC_MOBYS_MOBY_SPAWN_H */
