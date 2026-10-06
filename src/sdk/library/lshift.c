/****************************************************************
 *
 * The author of this software is David M. Gay.
 *
 * Copyright (c) 1991 by AT&T.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR NOR AT&T MAKES ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 ***************************************************************/

/* Source: dtoa (David M. Gay / AT&T). */

#include "types.h"

struct Bigint {
    s32 _next;
    s32 _k;
    s32 _maxwds;
    s32 _sign;
    s32 _wds;
    u32 _x[1];
};

extern struct Bigint *_Balloc(s32, s32);
extern s32 InsertLinkObject();

struct Bigint *_lshift(s32 arg0, struct Bigint *arg1, s32 arg2) {
    s32 i;
    s32 k1;
    s32 n;
    s32 n1;
    struct Bigint *b1;
    u32 *x;
    u32 *x1;
    u32 *xe;
    u32 z;

    n = arg2 >> 5;
    k1 = arg1->_k;
    n1 = n + arg1->_wds + 1;
    for (i = arg1->_maxwds; n1 > i; i <<= 1) {
        k1++;
    }
    b1 = _Balloc(arg0, k1);
    x1 = b1->_x;
    for (i = 0; i < n; i++) {
        *x1++ = 0;
    }
    x = arg1->_x;
    xe = x + arg1->_wds;
    if (arg2 &= 0x1F) {
        k1 = 32 - arg2;
        z = 0;
        do {
            *x1++ = *x << arg2 | z;
            z = *x++ >> k1;
        } while (x < xe);
        if ((*x1 = z) != 0) {
            ++n1;
        }
    } else {
        do {
            *x1++ = *x++;
        } while (x < xe);
    }
    b1->_wds = n1 - 1;
    InsertLinkObject(arg0, arg1);
    return b1;
}
