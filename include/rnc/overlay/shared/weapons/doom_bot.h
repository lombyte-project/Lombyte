#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_DOOM_BOT_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_DOOM_BOT_H

#include "types.h"
#include "rnc/sdk/eetypes.h"

struct Moby;

void           doom_bot_update(struct Moby *) __asm__("FUN_L00_002bffa8");
unsigned char *spawn_doom_bot(int, u128 *, u128 *) __asm__("FUN_L00_002bfe08");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_DOOM_BOT_H */
