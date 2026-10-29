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

s32 CountLeadingZeroBits(s32 word) __asm__("_hi0bits");

s32 CountLeadingZeroBits(s32 word) {
    s32 bit_count;

    if (!(word & 0xffff0000)) {
        bit_count = 16;
        word <<= 16;
    } else {
        bit_count = 0;
    }
    if (!(word & 0xff000000)) {
        bit_count += 8;
        word <<= 8;
    }
    if (!(word & 0xf0000000)) {
        bit_count += 4;
        word <<= 4;
    }
    if (!(word & 0xc0000000)) {
        bit_count += 2;
        word <<= 2;
    }
    if (word >= 0) {
        bit_count += 1;
        if (!(word & 0x40000000)) {
            return 32;
        }
    }
    return bit_count;
}
