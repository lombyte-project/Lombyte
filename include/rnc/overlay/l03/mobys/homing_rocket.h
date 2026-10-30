#ifndef LOMBYTE_RNC_OVERLAY_L03_MOBYS_HOMING_ROCKET_H
#define LOMBYTE_RNC_OVERLAY_L03_MOBYS_HOMING_ROCKET_H

#include "types.h"

struct Moby;

/* Spawns a homing rocket (class 0x341) at pos, heading along heading_vector, fired by parent
   (kerwan_path_enemy.c). Returns the moby or NULL. */
char *spawn_homing_rocket(void        *heading_vector,
                          void        *pos,
                          struct Moby *parent,
                          int          timer_frames,
                          float        initial_heading,
                          float        initial_speed,
                          float        unread_value) __asm__("FUN_L03_002d4288");

#endif /* LOMBYTE_RNC_OVERLAY_L03_MOBYS_HOMING_ROCKET_H */
