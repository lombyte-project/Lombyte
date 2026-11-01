#ifndef LOMBYTE_RNC_OVERLAY_L17_MOBYS_BLARG_SHIP_H
#define LOMBYTE_RNC_OVERLAY_L17_MOBYS_BLARG_SHIP_H

#include "types.h"

struct Moby;

void  blarg_ship_update(struct Moby *) __asm__("FUN_L17_002f40d8");
char *try_activate_blarg_ship_from_list(int) __asm__("FUN_L17_002f4fb8");

#endif /* LOMBYTE_RNC_OVERLAY_L17_MOBYS_BLARG_SHIP_H */
