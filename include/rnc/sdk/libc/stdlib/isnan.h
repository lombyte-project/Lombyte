#ifndef LOMBYTE_RNC_SDK_LIBC_STDLIB_ISNAN_H
#define LOMBYTE_RNC_SDK_LIBC_STDLIB_ISNAN_H

#include "types.h"

s32 isnan(f64) __asm__("ClassifyDoubleNaN");

#endif /* LOMBYTE_RNC_SDK_LIBC_STDLIB_ISNAN_H */
