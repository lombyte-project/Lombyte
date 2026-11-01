#ifndef LOMBYTE_RNC_SDK_LIBC_MALLOC__CALLOC_R_H
#define LOMBYTE_RNC_SDK_LIBC_MALLOC__CALLOC_R_H

#include "types.h"

struct _reent;

void *_calloc_r(struct _reent *, u32, u32);

#endif /* LOMBYTE_RNC_SDK_LIBC_MALLOC__CALLOC_R_H */
