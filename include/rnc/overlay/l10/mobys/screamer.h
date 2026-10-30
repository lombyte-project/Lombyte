#ifndef LOMBYTE_RNC_OVERLAY_L10_MOBYS_SCREAMER_H
#define LOMBYTE_RNC_OVERLAY_L10_MOBYS_SCREAMER_H

#include "types.h"
#include "rnc/mobys/moby.h"

/* Level 10 screamer: wakes the waiting members of its group (the toxic crabs call it too). */
void activate_waiting_moby_group_members(struct Moby *moby) __asm__("FUN_L10_002e0138");

#endif /* LOMBYTE_RNC_OVERLAY_L10_MOBYS_SCREAMER_H */
