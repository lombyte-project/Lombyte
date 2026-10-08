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

/* _fpadd_parts: stock GCC 2.95.2 fp-bit.c body; `thenan` is the SDK's global
 * data object at 0x1596F0 (config name D_001596F0). */

#include "types.h"
#include "rnc/sdk/library/fp_number.h"

extern FpNumber D_001596F0;

#define LSHIFT(a)                                                                                  \
    { a = (a & 1) | (a >> 1); }
#define IMPLICIT_1 0x1000000000000000ULL
#define IMPLICIT_2 0x2000000000000000ULL

FpNumber *_fpadd_parts(FpNumber *a, FpNumber *b, FpNumber *tmp) {
    s64 tfraction;
    s32 a_normal_exp;
    s32 b_normal_exp;
    u64 a_fraction;
    u64 b_fraction;

    if ((u32)a->class < 2) {
        return a;
    }
    if ((u32)b->class < 2) {
        return b;
    }
    if ((a->class ^ 4) == 0) {
        if ((b->class ^ 4) == 0 && a->sign != b->sign) {
            return &D_001596F0;
        }
        return a;
    }
    if ((b->class ^ 4) == 0) {
        return b;
    }
    if ((b->class ^ 2) == 0) {
        if ((a->class ^ 2) == 0) {
            *tmp = *a;
            tmp->sign = a->sign & b->sign;
            return tmp;
        }
        return a;
    }
    if ((a->class ^ 2) == 0) {
        return b;
    }

    a_normal_exp = a->normal_exp;
    b_normal_exp = b->normal_exp;
    a_fraction = a->fraction;
    b_fraction = b->fraction;

    {
        s32 diff;

        diff = a_normal_exp - b_normal_exp;
        if (diff < 0) {
            diff = -diff;
        }
        if (diff < 64) {
            while (a_normal_exp > b_normal_exp) {
                b_normal_exp++;
                LSHIFT(b_fraction);
            }
            while (b_normal_exp > a_normal_exp) {
                a_normal_exp++;
                LSHIFT(a_fraction);
            }
        } else {
            if (a_normal_exp > b_normal_exp) {
                b_normal_exp = a_normal_exp;
                b_fraction = 0;
            } else {
                a_normal_exp = b_normal_exp;
                a_fraction = 0;
            }
        }
    }

    if (a->sign != b->sign) {
        if (a->sign) {
            tfraction = -a_fraction + b_fraction;
        } else {
            tfraction = a_fraction - b_fraction;
        }
        if (tfraction >= 0) {
            tmp->sign = 0;
            tmp->normal_exp = a_normal_exp;
            tmp->fraction = tfraction;
        } else {
            tmp->sign = 1;
            tmp->normal_exp = a_normal_exp;
            tmp->fraction = -tfraction;
        }
        while (tmp->fraction < IMPLICIT_1 && tmp->fraction) {
            tmp->fraction <<= 1;
            tmp->normal_exp--;
        }
    } else {
        tmp->sign = a->sign;
        tmp->normal_exp = a_normal_exp;
        tmp->fraction = a_fraction + b_fraction;
    }
    tmp->class = 3;
    if (tmp->fraction >= IMPLICIT_2) {
        LSHIFT(tmp->fraction);
        tmp->normal_exp++;
    }
    return tmp;
}
