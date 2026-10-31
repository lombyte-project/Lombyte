#ifndef LOMBYTE_RNC_SDK_LIBC_MPREC__DTOA_R_H
#define LOMBYTE_RNC_SDK_LIBC_MPREC__DTOA_R_H

#include "types.h"

struct _reent;

char *_dtoa_r(struct _reent *, f64, s32, s32, s32 *, s32 *, char **);

#endif /* LOMBYTE_RNC_SDK_LIBC_MPREC__DTOA_R_H */
