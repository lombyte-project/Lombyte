#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_LARGE_AMOEBOID_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_LARGE_AMOEBOID_H

#include "types.h"

struct Moby;

int  cancel_suck_cannon_suction_and_set_state_14(struct Moby *) __asm__("FUN_L01_002efb98");
void get_moby_data_plus_0xc0() __asm__("FUN_L01_002efbc8");
void large_amoeboid_update(unsigned char *) __asm__("FUN_L01_002edca0");
int  moby_hold_state_if_found(struct Moby *) __asm__("FUN_L01_002efaf8");
int  process_amoeboid_suck_cannon_suction(struct Moby *) __asm__("FUN_L01_002efa88");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_LARGE_AMOEBOID_H */
