#ifndef LOMBYTE_RNC_SDK_LIBC_MALLOC_H
#define LOMBYTE_RNC_SDK_LIBC_MALLOC_H

#include "types.h"

struct _reent;

void *_calloc_r(struct _reent *, u32, u32);
void *SbrkReentrant(void *, int) __asm__("_sbrk_r");
void  _free_r(struct _reent *, void *);
void  malloc_extend_top(struct _reent *, u32);
void *_malloc_r(struct _reent *, u32);
s32   _malloc_trim_r(struct _reent *, u32);

#endif /* LOMBYTE_RNC_SDK_LIBC_MALLOC_H */
