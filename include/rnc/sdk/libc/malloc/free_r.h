#ifndef LOMBYTE_RNC_SDK_LIBC_MALLOC_FREE_R_H
#define LOMBYTE_RNC_SDK_LIBC_MALLOC_FREE_R_H

#include "types.h"

struct _reent;

void _free_r(struct _reent *, void *);

#endif /* LOMBYTE_RNC_SDK_LIBC_MALLOC_FREE_R_H */
