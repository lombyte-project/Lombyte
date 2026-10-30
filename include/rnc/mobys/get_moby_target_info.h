#ifndef LOMBYTE_RNC_MOBYS_GET_MOBY_TARGET_INFO_H
#define LOMBYTE_RNC_MOBYS_GET_MOBY_TARGET_INFO_H

#include "types.h"

/* A moby's target info record (pvars[0], hit state and aim bounds); 0 unless flag 0x20 is set. */
int get_moby_target_info(char *moby) __asm__("FUN_002141f8");

#endif /* LOMBYTE_RNC_MOBYS_GET_MOBY_TARGET_INFO_H */
