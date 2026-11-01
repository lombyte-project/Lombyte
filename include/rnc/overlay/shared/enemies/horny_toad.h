#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_HORNY_TOAD_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_HORNY_TOAD_H

#include "types.h"

struct Moby;

int  cancel_suck_cannon_suction_and_set_moby_state_9(char *) __asm__("FUN_L00_002d57d0");
void get_moby_private_data_d0_address() __asm__("FUN_L00_002d5800");
void horny_toad_update() __asm__("FUN_L00_002d4610");
int  process_suck_cannon_suction_and_update_moby_state(char *) __asm__("FUN_L00_002d56e0");
void collect_moby_and_clear_path_keys(struct Moby *) __asm__("FUN_L00_002d5830");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_HORNY_TOAD_H */
