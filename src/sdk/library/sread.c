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

/* Source: newlib (UC Berkeley). */

#include "types.h"
struct StdioFile {
    u8 pad_0[0xC];
    u16 unkC;
    s32 unkE;
    u8 pad_12[0x3E];
    s32 unk50;
    s32 unk54;
};

extern s64 reentrant_read() __asm__("func_00116108");
s64 __sread(struct StdioFile *fp, s32 buf, s32 len) {
    register s32 bytes_read;

    bytes_read =
        (s32)reentrant_read(*(s32 *)((u8 *)fp + 0x54), *(s16 *)((u8 *)fp + 0xE), buf, len);
    if (bytes_read >= 0) {
        goto block_2;
    }
    goto block_4;
block_2:
    *(s32 *)((u8 *)fp + 0x50) = *(s32 *)((u8 *)fp + 0x50) + bytes_read;
    goto block_5;
block_4:
    fp->unkC = (u16)(fp->unkC & 0xEFFF);
block_5:
    return bytes_read;
}
