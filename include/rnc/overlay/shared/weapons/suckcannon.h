#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_SUCKCANNON_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_SUCKCANNON_H

#include "types.h"

void *get_moby_suck_cannon_record(void *) __asm__("FUN_L00_002db890");

struct Moby;

void suckcannon_update() __asm__("FUN_L00_002da790");
s32  cancel_suck_cannon_suction(struct Moby *) __asm__("FUN_L00_002dbe20");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_SUCKCANNON_H */
