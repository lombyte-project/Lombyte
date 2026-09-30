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

typedef struct _Bigint {
    struct _Bigint *_next;
    s32 _k;
    s32 _maxwds;
    s32 _sign;
    s32 _wds;
    u32 _x[1];
} _Bigint;

extern s32 InsertLinkObject();
extern _Bigint *_Balloc();
extern s32 memcpy();

#define Bcopy(x, y) memcpy((char *)&x->_sign, (char *)&y->_sign, y->_wds * sizeof(s32) + 2 * sizeof(s32))

_Bigint *_multadd(s32 ptr, _Bigint *b, s32 m, s32 a)
{
    s32 i, wds;
    u32 *x, y;
    u32 xi, z;
    _Bigint *b1;

    wds = b->_wds;
    x = b->_x;
    i = 0;
    do {
        xi = *x;
        y = (xi & 0xffff) * m + a;
        z = (xi >> 16) * m + (y >> 16);
        a = (s32)(z >> 16);
        *x++ = (z << 16) + (y & 0xffff);
    } while (++i < wds);
    if (a) {
        if (wds >= b->_maxwds) {
            b1 = _Balloc(ptr, b->_k + 1);
            Bcopy(b1, b);
            InsertLinkObject(ptr, b);
            b = b1;
        }
        b->_x[wds++] = a;
        b->_wds = wds;
    }
    return b;
}
