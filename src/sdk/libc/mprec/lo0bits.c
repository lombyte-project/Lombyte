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

s32 CountTrailingZeroBits(u32 *value) __asm__("_lo0bits");

s32 CountTrailingZeroBits(u32 *value) {
    u32 word = *value;
    s32 bit_count;

    if (word & 7) {
        if (word & 1) {
            return 0;
        }
        if (word & 2) {
            word >>= 1;
            *value = word;
            return 1;
        }
        word >>= 2;
        *value = word;
        return 2;
    }

    if (!(word & 0xffff)) {
        bit_count = 16;
        word >>= 16;
    } else {
        bit_count = 0;
    }
    if (!(word & 0xff)) {
        bit_count += 8;
        word >>= 8;
    }
    if (!(word & 0xf)) {
        bit_count += 4;
        word >>= 4;
    }
    if (!(word & 3)) {
        bit_count += 2;
        word >>= 2;
    }
    if (!(word & 1)) {
        word >>= 1;
        bit_count += 1;
        if (!word) {
            return 32;
        }
    }
    *value = word;
    return bit_count;
}
