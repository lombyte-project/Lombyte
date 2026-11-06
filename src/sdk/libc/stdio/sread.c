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

/* newlib struct __sFILE */
struct StdioFile {
    u8  pad_0[0xC];
    u16 _flags;
    s16 _file;
    u8  pad_10[0x40];
    s32 _offset;
    s32 _data;
};

extern s64 _read_r() __asm__("func_00116108");

s64 __sread(struct StdioFile *fp, s32 destination_buffer, s32 max_bytes) {
    register s32 bytes_read;

    bytes_read = (s32)_read_r(fp->_data, fp->_file, destination_buffer, max_bytes);
    if (bytes_read >= 0) {
        goto block_2;
    }
    goto block_4;
block_2:
    fp->_offset = fp->_offset + bytes_read;
    goto block_5;
block_4:
    fp->_flags = (u16)(fp->_flags & 0xEFFF);
block_5:
    return bytes_read;
}
