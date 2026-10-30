#ifndef LOMBYTE_RNC_OVERLAY_L14_MOBYS_ENEMY_FIGHTER_H
#define LOMBYTE_RNC_OVERLAY_L14_MOBYS_ENEMY_FIGHTER_H

#include "types.h"
#include "rnc/mobys/moby.h"

/* Enemy fighters shot down so far (both fighter classes count; the third one plays a dialogue). */
extern int enemy_fighter_destruction_count __asm__("D_0015EE08");

void initialize_flight_path_state(struct Moby *moby) __asm__("FUN_L14_002fc890");

#endif /* LOMBYTE_RNC_OVERLAY_L14_MOBYS_ENEMY_FIGHTER_H */
