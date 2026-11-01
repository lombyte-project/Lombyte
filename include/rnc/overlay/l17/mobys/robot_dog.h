#ifndef LOMBYTE_RNC_OVERLAY_L17_MOBYS_ROBOT_DOG_H
#define LOMBYTE_RNC_OVERLAY_L17_MOBYS_ROBOT_DOG_H

#include "types.h"

struct Moby;

int  cancel_suck_cannon_suction_and_set_state_12(struct Moby *) __asm__("FUN_L17_002f0040");
int  enemy_stun_update(struct Moby *) __asm__("FUN_L17_002efea8");
void robot_dog_update(unsigned char *) __asm__("FUN_L17_002eeb68");

#endif /* LOMBYTE_RNC_OVERLAY_L17_MOBYS_ROBOT_DOG_H */
