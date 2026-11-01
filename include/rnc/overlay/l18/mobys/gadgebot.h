#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_GADGEBOT_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_GADGEBOT_H

#include "types.h"

struct Moby;

void gadgebot_update(unsigned char *) __asm__("FUN_L18_002fbb98");
void hide_moby_and_clear_collision(struct Moby *) __asm__("FUN_L18_002fcd00");
int  update_moby_suck_cannon_state(struct Moby *) __asm__("FUN_L18_002fcbb0");

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_GADGEBOT_H */
