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

struct _reent;

typedef union {
    f64 d;
    u32 i[2];
} double_union;

#define word0(x) ((x).i[1])
#define Sign_bit 0x80000000U
#define ALT      0x001

extern s8 *_dtoa_r(struct _reent *ptr, f64 _d, s32 mode, s32 ndigits, s32 *decpt, s32 *sign,
                   s8 **rve);

static s8 *cvt(struct _reent *data, f64 value, s32 ndigits, s32 flags, s8 *sign, s32 *decpt, s32 ch,
               s32 *length) {
    s32 mode, dsgn;
    s8 *digits, *bp, *rve;
    double_union tmp;

    if (ch == 'f') {
        mode = 3;
    } else {
        if (ch == 'e' || ch == 'E') {
            ndigits++;
        }
        mode = 2;
    }
    tmp.d = value;
    if (word0(tmp) & Sign_bit) {
        value = -value;
        *sign = '-';
    } else {
        *sign = '\000';
    }
    digits = _dtoa_r(data, value, mode, ndigits, decpt, &dsgn, &rve);
    if ((ch != 'g' && ch != 'G') || (flags & ALT)) {
        bp = digits + ndigits;
        if (ch == 'f') {
            if (*digits == '0' && value) {
                *decpt = -ndigits + 1;
            }
            bp += *decpt;
        }
        if (value == 0) {
            rve = bp;
        }
        while (rve < bp) {
            *rve++ = '0';
        }
    }
    *length = (s32)(rve - digits);
    return digits;
}
