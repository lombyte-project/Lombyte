#ifndef LOMBYTE_RNC_GAME_TRANSITIONS_H
#define LOMBYTE_RNC_GAME_TRANSITIONS_H

#include "types.h"

void request_space_load(s32);
void transition_load_wad(void) __asm__("FUN_001ea830");

#endif /* LOMBYTE_RNC_GAME_TRANSITIONS_H */
