#ifndef LOMBYTE_EE_COP2_H
#define LOMBYTE_EE_COP2_H

#include "types.h"

/* Low-level EE access, not a game function or a memory-mapped register.
 * Read COP2 control register 29 without the interlock form of cfc2. The
 * following nop retains the observed read latency slot. The memory clobber
 * prevents surrounding volatile MMIO observations from crossing this read.
 * Only the polling algorithm that uses this accessor is descriptive C;
 * this accessor necessarily remains an explicit hardware instruction.
 */
static __inline__ u32 ee_read_vpu_stat(void) {
    u32 value;
    __asm__ __volatile__("cfc2.ni %0,$29\n\tnop" : "=r"(value) : : "memory");
    return value;
}

#endif /* LOMBYTE_EE_COP2_H */
