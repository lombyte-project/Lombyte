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

typedef struct FloatParts {
    s32 category;
    s32 sign;
    s32 exponent;
    u32 mantissa;
} FloatParts;

extern void UnpackFloat(const f32 *input_value, FloatParts *parts) __asm__("__unpack_f");
extern u64 MakeDoublePacket(s32 category, s32 sign, s32 exponent,
                            u64 mantissa) __asm__("__make_dp");

u64 ConvertFloatToPackedDouble(f32 input_value) __asm__("fptodp");

u64 ConvertFloatToPackedDouble(f32 input_value) {
    FloatParts parts;

    UnpackFloat(&input_value, &parts);
    return MakeDoublePacket(parts.category, parts.sign, parts.exponent,
                            ((u64)parts.mantissa << 32) >> 2);
}
