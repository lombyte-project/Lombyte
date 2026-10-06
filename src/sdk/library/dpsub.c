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
extern s32 __pack_d();
extern s32 __unpack_d();
extern s32 _fpadd_parts();
struct DpsubWork {
    u8 pad_0[0x20];
    s32 unk20;
    s32 unk24;
    u8 pad_28[0x18];
    s32 unk40;
    u8 pad_44[0x1C];
    s64 unk60;
    s64 unk68;
};
void dpsub(s64 arg0, s64 arg1) {
    struct DpsubWork work;

    work.unk60 = arg0;
    work.unk68 = arg1;
    __unpack_d(&work.unk60, &work);
    __unpack_d(&work.unk68, &work.unk20);
    work.unk24 ^= 1;
    __pack_d(_fpadd_parts(&work, &work.unk20, &work.unk40));
}
