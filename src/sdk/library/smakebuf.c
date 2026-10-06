/* No user fns here.  Pesch 15apr92. */

/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "types.h"
#include "rnc/sdk/library/stdio.h"

/* Only mode is accessed here; the remaining SDK stat fields are opaque.
 * Retail reserves 0x70 bytes before the saved registers. */
struct RncStatBuffer {
    u16 st_dev;
    u16 st_ino;
    u32 st_mode;
    u8 remaining[0x68];
};
extern u8 D_00113AC8[];
extern void *_malloc_r();
extern s32 reentrant_syscall_with_two_arguments() __asm__("func_00114000");
extern s32 func_00118EC0();
extern s32 __sseek(void *, s32, s32);

void __smakebuf(RncFile *fp) {
    u32 size, couldbetty;
    void *p;
    struct RncStatBuffer st;

    if (fp->_flags & 2) {
        fp->_bf._base = fp->_p = fp->_nbuf;
        fp->_bf._size = 1;
        return;
    }
    if ((s16)fp->_file < 0 ||
        reentrant_syscall_with_two_arguments(fp->_data, (s16)fp->_file, &st) < 0) {
        couldbetty = 0;
        size = 0x400;
        fp->_flags |= 0x800;
    } else {
        couldbetty = (st.st_mode & 0xF000) == 0x2000;
        size = 0x400;
        if ((st.st_mode & 0xF000) == 0x8000 && fp->_seek == __sseek) {
            fp->_flags |= 0x400;
            fp->_blksize = 1024;
        } else {
            fp->_flags |= 0x800;
        }
    }
    if ((p = _malloc_r(fp->_data, size)) == NULL) {
        fp->_flags |= 2;
        fp->_bf._base = fp->_p = fp->_nbuf;
        fp->_bf._size = 1;
    } else {
        fp->_data->__cleanup = (void (*)(RncReent *))D_00113AC8;
        fp->_flags |= 0x80;
        fp->_bf._base = fp->_p = (u8 *)p;
        fp->_bf._size = size;
        if (couldbetty && func_00118EC0((s16)fp->_file))
            fp->_flags |= 1;
    }
}
