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

/* dptoul: fp-bit float_to_usi (bitfield-era variant), else-if chain as in the era source. */

#include "types.h"
#include "rnc/sdk/library/fp_number.h"

typedef union {
    f64 value;
    u64 value_raw;
} FLOUnion;

void __unpack_d(FLOUnion *src, FpNumber *dst);

u32 dptoul(u64 arg_a) {
    FpNumber a;

    __unpack_d((FLOUnion *)&arg_a, &a);
    if ((a.class ^ 2) == 0) {
        return 0;
    }
    if ((u32)a.class < 2) {
        return 0;
    }
    if (a.sign) {
        return 0;
    }
    if ((a.class ^ 4) == 0) {
        return 0xFFFFFFFF;
    }
    if (a.normal_exp < 0) {
        return 0;
    }
    if (a.normal_exp > 31) {
        return 0xFFFFFFFF;
    } else if (a.normal_exp > 60) {
        return (s32)(a.fraction << (a.normal_exp - 60));
    } else {
        return (s32)(a.fraction >> (60 - a.normal_exp));
    }
}
