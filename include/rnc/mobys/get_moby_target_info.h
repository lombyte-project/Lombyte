#ifndef LOMBYTE_RNC_MOBYS_GET_MOBY_TARGET_INFO_H
#define LOMBYTE_RNC_MOBYS_GET_MOBY_TARGET_INFO_H

#include "types.h"

/* The record pvars[0] points to on mobys with flag 0x20: hit state, aim bounds. */
int get_moby_target_info(char *moby) __asm__("FUN_002141f8");

#endif /* LOMBYTE_RNC_MOBYS_GET_MOBY_TARGET_INFO_H */
