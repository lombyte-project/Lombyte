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

/* Descriptive C recovery of the GCC fp-bit __unpack_d runtime helper. */

#include "types.h"

typedef struct FpNumber {
    s32 class;
    u32 sign;
    s32 normal_exp;
    s32 alignment_padding;
    u64 fraction;
} FpNumber;

typedef union {
    double value;
    u64 value_raw;
} FLOUnion;

void __unpack_d(FLOUnion *src, FpNumber *dst);

void __unpack_d(FLOUnion *src, FpNumber *dst) {
    u64 fraction = src->value_raw & 0x000FFFFFFFFFFFFFULL;
    s32 exp = ((s32)(src->value_raw >> 52)) & 0x7FF;
    s32 sign = ((s32)(src->value_raw >> 63)) & 1;
    dst->sign = sign;
    if (exp == 0) {
        do {
        } while (0);
        dst->class = 2;
    } else if (exp == 0x7FF) {
        if (fraction == 0) {
            dst->class = 4;
        } else {
            if (fraction & 0x8000000000000ULL) {
                dst->class = 1;
            } else {
                dst->class = 0;
            }
            dst->fraction = fraction;
        }
    } else {
        dst->normal_exp = exp - 1023;
        dst->class = 3;
        dst->fraction = (fraction << 8) | 0x1000000000000000ULL;
    }
}
