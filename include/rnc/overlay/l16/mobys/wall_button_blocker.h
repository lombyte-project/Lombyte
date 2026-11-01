#ifndef LOMBYTE_RNC_OVERLAY_L16_MOBYS_WALL_BUTTON_BLOCKER_H
#define LOMBYTE_RNC_OVERLAY_L16_MOBYS_WALL_BUTTON_BLOCKER_H

#include "types.h"

struct Moby;

void wall_button_blocker_update(struct Moby *) __asm__("FUN_L16_002e36e8");
void set_wall_button_blocker_collision(void *, int) __asm__("FUN_L16_002e3770");

#endif /* LOMBYTE_RNC_OVERLAY_L16_MOBYS_WALL_BUTTON_BLOCKER_H */
