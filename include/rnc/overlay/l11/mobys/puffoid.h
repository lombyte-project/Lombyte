#ifndef LOMBYTE_RNC_OVERLAY_L11_MOBYS_PUFFOID_H
#define LOMBYTE_RNC_OVERLAY_L11_MOBYS_PUFFOID_H

#include "types.h"

/* Level 11 puffoid: the state test bob's boat and the resort owner use to skip puffoids
   that are being sucked, hit or killed. */
s32 is_moby_in_bob_target_exclusion_state(const u8 *moby) __asm__("FUN_L11_00316128");

#endif /* LOMBYTE_RNC_OVERLAY_L11_MOBYS_PUFFOID_H */
