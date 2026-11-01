#ifndef LOMBYTE_RNC_SDK_LIBC_STDLIB_RAND_H
#define LOMBYTE_RNC_SDK_LIBC_STDLIB_RAND_H

#include "types.h"

s32 RandomNext(void) __asm__("rand");

#endif /* LOMBYTE_RNC_SDK_LIBC_STDLIB_RAND_H */
