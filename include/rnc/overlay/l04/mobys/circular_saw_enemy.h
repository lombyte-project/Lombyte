#ifndef LOMBYTE_RNC_OVERLAY_L04_MOBYS_CIRCULAR_SAW_ENEMY_H
#define LOMBYTE_RNC_OVERLAY_L04_MOBYS_CIRCULAR_SAW_ENEMY_H

#include "types.h"

struct Moby;

void circular_saw_enemy_update() __asm__("FUN_L04_002c2270");
void get_circular_saw_enemy_suck_cannon_record() __asm__("FUN_L04_002c41b0");
int  launch_circular_saw_enemy_from_suck_cannon(struct Moby *,
                                                int,
                                                int,
                                                float) __asm__("FUN_L04_002c40d0");

#endif /* LOMBYTE_RNC_OVERLAY_L04_MOBYS_CIRCULAR_SAW_ENEMY_H */
