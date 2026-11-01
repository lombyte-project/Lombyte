#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_MINE_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_MINE_H

#include "types.h"

struct Moby;

void hide_moby_and_disable_collision(struct Moby *) __asm__("FUN_L18_002d6280");
void moby_568_update(struct Moby *) __asm__("FUN_L18_002d5918");
int  moby_hold_state_while_active(struct Moby *) __asm__("FUN_L18_002d6190");
int  update_moby_state_pause(struct Moby *) __asm__("FUN_L18_002d6108");

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_MINE_H */
