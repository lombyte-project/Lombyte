#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_FROG_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_FROG_H

#include "types.h"

struct Moby;

s32  cancel_moby_suction_and_save_state(unsigned char *) __asm__("FUN_L06_002ea7c8");
void frog_update(unsigned char *) __asm__("FUN_L06_002e8678");
int  update_moby_grab_state(struct Moby *) __asm__("FUN_L06_002ea5e0");
int  update_moby_suck_cannon_storage_state(unsigned char *) __asm__("FUN_L06_002ea6a8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_FROG_H */
