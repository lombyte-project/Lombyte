/* Copyright (C) 1989, 92-98, 1999 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* As a special exception, if you link this library with other files,
   some of which are compiled with GCC, to produce an executable,
   this library does not by itself cause the resulting executable
   to be covered by the GNU General Public License.
   This exception does not however invalidate any other reasons why
   the executable file might be covered by the GNU General Public License.  */

/* Source: GCC 2.95.2 libgcc2.c. */

/* __fixunsdfdi: libgcc2 L_fixunsdfdi, verbatim arithmetic. */

#include "types.h"

u64 __fixunsdfdi(f64 a) {
    f64 b;
    u64 v;
    u64 high_word_coeff;

    high_word_coeff = 1ULL << 32;
    if (a < 0.0) {
        return 0;
    }
    b = a / 4294967296.0;
    v = (u32)b;
    v <<= 32;
    a -= (f64)v;
    if (a < 0.0) {
        v -= (u32)(-a);
    } else {
        v += (u32)a;
    }
    return v;
}
