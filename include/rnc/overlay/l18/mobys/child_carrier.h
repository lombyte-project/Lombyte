#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_CHILD_CARRIER_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_CHILD_CARRIER_H

#include "types.h"

void child_carrier_update(char *) __asm__("FUN_L18_002fa728");
void set_group_child_carrier_byte_bc(int, int) __asm__("FUN_L18_002fa888");

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_CHILD_CARRIER_H */
