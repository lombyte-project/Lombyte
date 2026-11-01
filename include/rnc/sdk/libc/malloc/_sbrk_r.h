#ifndef LOMBYTE_RNC_SDK_LIBC_MALLOC__SBRK_R_H
#define LOMBYTE_RNC_SDK_LIBC_MALLOC__SBRK_R_H

#include "types.h"

void *SbrkReentrant(void *, int) __asm__("_sbrk_r");

#endif /* LOMBYTE_RNC_SDK_LIBC_MALLOC__SBRK_R_H */
