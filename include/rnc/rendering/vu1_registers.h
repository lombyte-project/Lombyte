#ifndef LOMBYTE_RNC_RENDERING_VU1_REGISTERS_H
#define LOMBYTE_RNC_RENDERING_VU1_REGISTERS_H

#include "types.h"

/* Queues a GS register write in the VU1 packet. */
void vu1_add_g_sregister(s32 register_id, s64 value) __asm__("FUN_00233980");

#endif /* LOMBYTE_RNC_RENDERING_VU1_REGISTERS_H */
