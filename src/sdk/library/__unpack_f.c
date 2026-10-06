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

typedef struct FpSingle {
    s32 class;
    u32 sign;
    s32 normal_exp;
    u32 fraction;
} FpSingle;

typedef union {
    f32 value;
    struct {
        u32 fraction : 23 __attribute__((packed));
        u32 exp : 8 __attribute__((packed));
        u32 sign : 1 __attribute__((packed));
    } bits;
} FLOSingleUnion;

void __unpack_f(FLOSingleUnion *src, FpSingle *dst) {
    u32 fraction = src->bits.fraction;
    s32 exp = src->bits.exp;
    s32 sign = src->bits.sign;
    dst->sign = sign;
    if (exp == 0) {
        dst->class = 2;
    } else if (exp == 0xFF) {
        if (fraction == 0) {
            dst->class = 4;
        } else {
            if (fraction & 0x100000) {
                dst->class = 1;
            } else {
                dst->class = 0;
            }
            dst->fraction = fraction;
        }
    } else {
        dst->normal_exp = exp - 0x7F;
        dst->class = 3;
        dst->fraction = (fraction << 7) | 0x40000000;
    }
}
