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

/* dpdiv: fp-bit divide (bitfield-era variant) with _fpdiv_parts inlined. */

#include "types.h"
#include "rnc/sdk/library/fp_number.h"

typedef union {
    f64 value;
    u64 value_raw;
} FLOUnion;

extern FpNumber D_001596F0;
static __inline__ int IsNaN(FpNumber *x) {
    return (u32)x->class < 2;
}
static __inline__ int IsInf(FpNumber *x) {
    return (x->class ^ 4) == 0;
}
static __inline__ int IsZero(FpNumber *x) {
    return (x->class ^ 2) == 0;
}

void __unpack_d(FLOUnion *src, FpNumber *dst);
u64 __pack_d(FpNumber *src);

static __inline__ FpNumber *divide_parts(FpNumber *a, FpNumber *b) {
    u64 bit;
    u64 numerator;
    u64 denominator;
    u64 quotient;

    if (IsNaN(a)) {
        return a;
    }
    if (IsNaN(b)) {
        return b;
    }
    a->sign = a->sign ^ b->sign;
    if (IsInf(a) || IsZero(a)) {
        if (a->class == b->class) {
            return &D_001596F0;
        }
        return a;
    }
    if (IsInf(b)) {
        a->fraction = 0;
        a->normal_exp = 0;
        return a;
    }
    if (IsZero(b)) {
        a->class = 4;
        return a;
    }
    a->normal_exp = a->normal_exp - b->normal_exp;
    numerator = a->fraction;
    denominator = b->fraction;
    if (numerator < denominator) {
        numerator *= 2;
        a->normal_exp--;
    }
    bit = 0x1000000000000000ULL;
    quotient = 0;
    while (bit) {
        if (numerator >= denominator) {
            quotient |= bit;
            numerator -= denominator;
        }
        bit >>= 1;
        numerator *= 2;
    }
    if ((quotient & 0xFF) == 0x80) {
        if (quotient & 0x100) {
            quotient += 0x80;
        } else if (numerator) {
            quotient += 0x80;
        }
    }
    a->fraction = quotient;
    return a;
}

u64 dpdiv(u64 arg_a, u64 arg_b) {
    FpNumber a;
    FpNumber b;
    FpNumber *res;

    __unpack_d((FLOUnion *)&arg_a, &a);
    __unpack_d((FLOUnion *)&arg_b, &b);
    res = divide_parts(&a, &b);
    return __pack_d(res);
}
