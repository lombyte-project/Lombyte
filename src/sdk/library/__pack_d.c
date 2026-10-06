/* This is a software floating point library which can be used instead of
   the floating point routines in libgcc1.c for targets without hardware
   floating point. 
 Copyright (C) 1994, 1995, 1996, 1997, 1998 Free Software Foundation, Inc.

This file is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file with other programs, and to distribute
those programs without any restriction coming from the use of this
file.  (The General Public License restrictions do apply in other
respects; for example, they cover modification of the file, and
distribution when not linked into another program.)

This file is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* As a special exception, if you link this library with other files,
   some of which are compiled with GCC, to produce an executable,
   this library does not by itself cause the resulting executable
   to be covered by the GNU General Public License.
   This exception does not however invalidate any other reasons why
   the executable file might be covered by the GNU General Public License.  */

/* Source: GCC 2.95.2 fp-bit.c. */

#include "types.h"

typedef struct FpNumber {
    s32 class;
    u32 sign;
    s32 normal_exp;
    s32 alignment_padding;
    u64 fraction;
} FpNumber;

typedef union {
    f64 value;
    struct {
        u64 fraction : 52 __attribute__((packed));
        u32 exp : 11 __attribute__((packed));
        u32 sign : 1 __attribute__((packed));
    } bits;
} FLOUnion;

f64 __pack_d(FpNumber *src) {
    FLOUnion dst;
    u64 fraction = src->fraction;
    s32 sign = src->sign;
    s32 exp = 0;

    if (src->class == 0 || src->class == 1) {
        exp = 0x7FF;
        fraction |= 0x8000000000000ULL;
    } else if ((src->class ^ 4) == 0) {
        exp = 0x7FF;
        fraction = 0;
    } else if ((src->class ^ 2) == 0) {
        fraction = 0;
    } else if (fraction == 0) {
        exp = 0;
    } else {
        if (src->normal_exp < -0x3FE) {
            s32 shift = -0x3FE - src->normal_exp;
            exp = 0;
            if (shift > 56) {
                fraction = 0;
            } else {
                fraction >>= shift;
            }
            fraction >>= 8;
        } else if (src->normal_exp > 0x3FF) {
            exp = 0x7FF;
            fraction = 0;
        } else {
            exp = src->normal_exp + 0x3FF;
            if ((fraction & 0xFF) == 0x80) {
                if (fraction & 0x100) {
                    fraction += 0x80;
                }
            } else {
                fraction += 0x7F;
            }
            if (fraction >= 0x2000000000000000ULL) {
                fraction >>= 1;
                exp += 1;
            }
            fraction >>= 8;
        }
    }
    dst.bits.fraction = fraction;
    dst.bits.exp = exp;
    dst.bits.sign = sign;
    return dst.value;
}
