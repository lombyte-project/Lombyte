#ifndef LOMBYTE_RNC_GAMEPLAY_STATE_LEVEL_STATE_H
#define LOMBYTE_RNC_GAMEPLAY_STATE_LEVEL_STATE_H

#include "types.h"
#include "sda.h"

/*
 * Per-level flags of the save state, one byte per level index. The level
 * loops (map selection, marker drawing, memory card save) run to index 19,
 * and the two tables sit 0x18 bytes apart, each padded to a word.
 */
extern u8 level_available[20] __asm__("D_0013DD40") NOT_SDA;      /* shows the level on the map */
extern u8 level_visit_state[20] __asm__("D_0013DD58") NOT_SDA;    /* 0 new, 1 entered, 2 done */

#endif /* LOMBYTE_RNC_GAMEPLAY_STATE_LEVEL_STATE_H */
