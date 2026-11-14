/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Source: newlib (UC Berkeley). */

/* cvt: newlib-2000-02-17 vfprintf.c floating-point conversion helper. */

#include "types.h"
#include "rnc/sdk/libc/mprec.h"

struct _reent;

typedef union {
    f64 d;
    u32 i[2];
} double_union;

#define word0(x) ((x).i[1])
#define Sign_bit 0x80000000U
#define ALT      0x001

static s8 *cvt(struct _reent *reentrancy_state,
               f64            number_to_format,
               s32            ndigits,
               s32            format_flags,
               s8            *sign,
               s32           *decpt,
               s32            ch,

               s32 *length) {
    enum {
        DTOA_MODE_SIGNIFICANT_DIGITS = 2,
        DTOA_MODE_FRACTIONAL_DIGITS = 3
    };
    s32          dtoa_mode, dsgn;
    s8          *digits, *bp, *rve;
    double_union value_bits;

    if (ch == 'f') {
        dtoa_mode = DTOA_MODE_FRACTIONAL_DIGITS;
    } else {
        if (ch == 'e' || ch == 'E') {
            ndigits++;
        }
        dtoa_mode = DTOA_MODE_SIGNIFICANT_DIGITS;
    }
    value_bits.d = number_to_format;
    if (word0(value_bits) & Sign_bit) {
        number_to_format = -number_to_format;
        *sign = '-';
    } else {
        *sign = '\000';
    }
    digits = _dtoa_r(reentrancy_state, number_to_format, dtoa_mode, ndigits, decpt, &dsgn, &rve);
    if ((ch != 'g' && ch != 'G') || (format_flags & ALT)) {
        bp = digits + ndigits;
        if (ch == 'f') {
            if (*digits == '0' && number_to_format) {
                *decpt = -ndigits + 1;
            }
            bp += *decpt;
        }
        if (number_to_format == 0) {
            rve = bp;
        }
        while (rve < bp) {
            *rve++ = '0';
        }
    }
    *length = (s32)(rve - digits);
    return digits;
}
