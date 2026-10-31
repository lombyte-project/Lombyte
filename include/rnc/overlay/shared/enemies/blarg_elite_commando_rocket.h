#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_BLARG_ELITE_COMMANDO_ROCKET_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_BLARG_ELITE_COMMANDO_ROCKET_H

#include "types.h"

char *spawn_blarg_elite_commando_rocket(char  *owner,
                                        float *initial_movement_vector,
                                        char  *spawn_position) __asm__("FUN_L16_002a09a0");

void blarg_elite_commando_rocket_update(unsigned char *) __asm__("FUN_L15_0029ac30");
void spawn_rocket_trail_spark(char *) __asm__("FUN_L15_0029ae18");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_BLARG_ELITE_COMMANDO_ROCKET_H */
