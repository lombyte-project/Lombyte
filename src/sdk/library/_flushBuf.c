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

/* _flushBuf f6: MPEG buffer flush with GS status polling and IPU idle wait. */

#include "types.h"

typedef struct FlushBufContext {
    u8 pad_0[0x818];
    s32 unk818;
    u8 pad_81C[0x1C];
    s32 unk838;
    s32 unk83C;
    u8 pad_840[0x18];
    s32 unk858;
} FlushBufContext;

extern s32 D_00132E70[];
extern s32 _dispatchMpegCbNodata();
extern s64 _waitIpuIdle64();

void _flushBuf(FlushBufContext *context, s32 arg1) {
    s32 value;
    s32 counter;
    u32 command;
    s32 table_value;
    s32 index;
    s64 idle_result;

    value = arg1;
    counter = 0;
    if ((*(volatile u32 *)0x10002010 & 0x80004000) == 0x80000000) {
        do {
            if (counter++ >= 0x1389) {
                _dispatchMpegCbNodata(context->unk858);
                counter = 0;
            }
        } while ((*(volatile u32 *)0x10002010 & 0x80004000) == 0x80000000);
    }

    command = value | 0x40000000;
    *(volatile u32 *)0x10002000 = command;
    index = command >> 0x1C;
    table_value = D_00132E70[index];
    context->unk818 = table_value;
    idle_result = _waitIpuIdle64(context, &D_00132E70[index]);
    context->unk838 = (s32)idle_result;
    context->unk83C = 0x20;
}
