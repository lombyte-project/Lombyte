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

/* _strtol_r: newlib-2000-02-17 strtol.c adapted to the game's 64-bit long and
 * ctype table D_00152201 (= _ctype_ + 1). */

#include "types.h"

typedef struct _reent {
    s32 _errno;
} _reent;

extern u8 D_00152201[];
extern u64 __umoddi3(u64, u64);
extern u64 __udivdi3(u64, u64);
extern u64 __muldi3(u64, u64);

#define _U 01
#define _L 02
#define _D 04
#define _S 0x08

#define isspace(c) ((D_00152201[(c)] & _S) != 0)
#define isdigit(c) ((D_00152201[(c)] & _D) != 0)
#define isalpha(c) ((D_00152201[(c)] & (_U | _L)) != 0)
#define isupper(c) ((D_00152201[(c)] & _U) != 0)

s64 _strtol_r(_reent *rptr, const s8 *nptr, s8 **endptr, s32 base) {
    register const s8 *s = nptr;
    register u64 acc;
    register s32 c;
    register u64 cutoff;
    register s32 neg = 0, any, cutlim;

    do {
        c = *s++;
    } while (isspace(c));
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else if (c == '+') {
        c = *s++;
    }
    if ((base == 0 || base == 16) && c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0) {
        base = c == '0' ? 8 : 10;
    }
    cutoff = neg ? -(u64)0x8000000000000000LL : (u64)0x7FFFFFFFFFFFFFFFLL;
    cutlim = (s32)(cutoff % (u64)base);
    cutoff /= (u64)base;
    for (acc = 0, any = 0;; c = *s++) {
        if (isdigit(c)) {
            c -= '0';
        } else if (isalpha(c)) {
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        } else {
            break;
        }
        if (c >= base) {
            break;
        }
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)) {
            any = -1;
        } else {
            any = 1;
            acc *= (u64)base;
            acc += (u64)c;
        }
    }
    if (any < 0) {
        acc = neg ? 0x8000000000000000ULL : (u64)0x7FFFFFFFFFFFFFFFLL;
        rptr->_errno = 34;
    } else if (neg) {
        acc = -acc;
    }
    if (endptr != 0) {
        *endptr = (s8 *)(any ? s - 1 : nptr);
    }
    return (s64)acc;
}
