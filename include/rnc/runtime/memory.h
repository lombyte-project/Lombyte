#ifndef LOMBYTE_RNC_RUNTIME_MEMORY_H
#define LOMBYTE_RNC_RUNTIME_MEMORY_H

#include "types.h"

int  init_ring_buffer(int) __asm__("func_00119568");
void unpack_spr_descriptor(void *) __asm__("FUN_001238a0");

#endif /* LOMBYTE_RNC_RUNTIME_MEMORY_H */
