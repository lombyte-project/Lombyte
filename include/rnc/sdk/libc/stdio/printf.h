#ifndef LOMBYTE_RNC_SDK_LIBC_STDIO_PRINTF_H
#define LOMBYTE_RNC_SDK_LIBC_STDIO_PRINTF_H

#include "types.h"

int Printf(const char *, ...) __asm__("printf");

#endif /* LOMBYTE_RNC_SDK_LIBC_STDIO_PRINTF_H */
