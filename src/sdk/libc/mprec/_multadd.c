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
    s32             _k;
    s32             _maxwds;
    s32             _sign;
    s32             _wds;
    u32             _x[1];
} _Bigint;

extern s32      _Bfree() __asm__("InsertLinkObject");
extern _Bigint *_Balloc();
extern s32      memcpy();

#define Bcopy(x, y) \
    memcpy((char *)&x->_sign, (char *)&y->_sign, y->_wds * sizeof(s32) + 2 * sizeof(s32))

_Bigint *_multadd(s32 reent_context, _Bigint *bigint_operand, s32 multiplier, s32 carry) {
    s32      i, wds;
    u32     *x, y;
    u32      xi, z;
    _Bigint *expanded_bigint;

    wds = bigint_operand->_wds;
    x = bigint_operand->_x;
    i = 0;
    do {
        xi = *x;
        y = (xi & 0xffff) * multiplier + carry;
        z = (xi >> 16) * multiplier + (y >> 16);
        carry = (s32)(z >> 16);
        *x++ = (z << 16) + (y & 0xffff);
    } while (++i < wds);
    if (carry) {
        if (wds >= bigint_operand->_maxwds) {
            expanded_bigint = _Balloc(reent_context, bigint_operand->_k + 1);
            Bcopy(expanded_bigint, bigint_operand);
            _Bfree(reent_context, bigint_operand);
            bigint_operand = expanded_bigint;
        }
        bigint_operand->_x[wds++] = carry;
        bigint_operand->_wds = wds;
    }
    return bigint_operand;
}
