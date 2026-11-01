#ifndef LOMBYTE_RNC_SDK_LIBC_STDLIB_H
#define LOMBYTE_RNC_SDK_LIBC_STDLIB_H

#include "types.h"

int *__errno(void) __asm__("func_001138A8");
s32  isinf(f64) __asm__("FUN_00116168");
s32  isnan(f64) __asm__("ClassifyDoubleNaN");
s32  RandomNext(void) __asm__("rand");
void srand(int) __asm__("func_001160C8");

#endif /* LOMBYTE_RNC_SDK_LIBC_STDLIB_H */
