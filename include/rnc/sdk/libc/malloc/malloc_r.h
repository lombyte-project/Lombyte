#ifndef LOMBYTE_RNC_SDK_LIBC_MALLOC_MALLOC_R_H
#define LOMBYTE_RNC_SDK_LIBC_MALLOC_MALLOC_R_H

#include "types.h"

struct _reent;

void *_malloc_r(struct _reent *, u32);

#endif /* LOMBYTE_RNC_SDK_LIBC_MALLOC_MALLOC_R_H */
