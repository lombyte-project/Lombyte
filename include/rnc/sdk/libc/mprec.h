#ifndef LOMBYTE_RNC_SDK_LIBC_MPREC_H
#define LOMBYTE_RNC_SDK_LIBC_MPREC_H

#include "types.h"

struct Bigint;
struct _reent;

char          *_dtoa_r(struct _reent *, f64, s32, s32, s32 *, s32 *, char **);
s32            CountLeadingZeroBits(s32) __asm__("_hi0bits");
s32            CountTrailingZeroBits(u32 *) __asm__("_lo0bits");
struct Bigint *_lshift(s32, struct Bigint *, s32);

#endif /* LOMBYTE_RNC_SDK_LIBC_MPREC_H */
