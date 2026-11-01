#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_SCOUTBOT_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_SCOUTBOT_H

#include "types.h"

struct Moby;

void *enter_scoutbot_return_home_state(struct Moby *) __asm__("FUN_L08_002d4050");
void  scoutbot_update() __asm__("FUN_L08_002d2af0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_SCOUTBOT_H */
