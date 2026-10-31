#ifndef LOMBYTE_RNC_SDK_LIBC_MALLOC_MALLOC_EXTEND_TOP_H
#define LOMBYTE_RNC_SDK_LIBC_MALLOC_MALLOC_EXTEND_TOP_H

#include "types.h"

struct _reent;

void malloc_extend_top(struct _reent *, u32);

#endif /* LOMBYTE_RNC_SDK_LIBC_MALLOC_MALLOC_EXTEND_TOP_H */
