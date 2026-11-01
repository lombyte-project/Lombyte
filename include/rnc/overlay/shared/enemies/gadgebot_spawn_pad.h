#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_GADGEBOT_SPAWN_PAD_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_GADGEBOT_SPAWN_PAD_H

#include "types.h"

char *spawn_gadgebot_pad(char *owner) __asm__("FUN_L06_002d8b98");

void gadgebot_spawn_pad_update(char *) __asm__("FUN_L06_002d8c20");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_GADGEBOT_SPAWN_PAD_H */
