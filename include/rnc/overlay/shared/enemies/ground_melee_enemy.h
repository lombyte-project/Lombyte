#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_GROUND_MELEE_ENEMY_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_GROUND_MELEE_ENEMY_H

#include "types.h"

struct Moby;

void get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void ground_melee_enemy_update() __asm__("FUN_L09_002e27d8");
int  moby_hold_state_if_grabbed(struct Moby *) __asm__("FUN_L01_002f1c78");
int  run_moby_helper_and_set_state_7(struct Moby *) __asm__("FUN_L01_002f1dc8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_GROUND_MELEE_ENEMY_H */
