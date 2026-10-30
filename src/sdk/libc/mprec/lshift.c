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
extern s32            _Bfree() __asm__("InsertLinkObject");

struct Bigint *_lshift(s32 bigint_context, struct Bigint *input_bigint, s32 shift_bits) {
    s32            i;
    s32            k1;
    s32            whole_word_shift;
    s32            result_word_count_plus_one;
    struct Bigint *result_bigint;
    u32           *x;
    u32           *x1;
    u32           *xe;
    u32            z;

    whole_word_shift = shift_bits >> 5;
    k1 = input_bigint->_k;
    result_word_count_plus_one = whole_word_shift + input_bigint->_wds + 1;
    for (i = input_bigint->_maxwds; result_word_count_plus_one > i; i <<= 1) {
        k1++;
    }
    result_bigint = _Balloc(bigint_context, k1);
    x1 = result_bigint->_x;
    for (i = 0; i < whole_word_shift; i++) {
        *x1++ = 0;
    }
    x = input_bigint->_x;
    xe = x + input_bigint->_wds;
    if (shift_bits &= 0x1F) {
        k1 = 32 - shift_bits;
        z = 0;
        do {
            *x1++ = *x << shift_bits | z;
            z = *x++ >> k1;
        } while (x < xe);
        if ((*x1 = z) != 0) {
            ++result_word_count_plus_one;
        }
    } else {
        do {
            *x1++ = *x++;
        } while (x < xe);
    }
    result_bigint->_wds = result_word_count_plus_one - 1;
    _Bfree(bigint_context, input_bigint);
    return result_bigint;
}
