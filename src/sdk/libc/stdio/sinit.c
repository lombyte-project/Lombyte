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
#include "rnc/data_labels.h"
#include "rnc/sdk/libc/stdio.h"

/* newlib struct _reent */
struct Reent {
    u8  pad_0[0x38];
    s32 __sdidinit;
    s32 __cleanup;
    u8  pad_40[0x198];
    s32 __sglue_next;
    s32 __sglue_niobs;
    s32 __sglue_iobs;
};

void __sinit(struct Reent *reent_context) {
    s32 *std_streams;

    std_streams = ((u8 *)reent_context + (0x1E4));
    reent_context->__cleanup = D_00113AC8;
    reent_context->__sdidinit = 1;
    std(std_streams, 4, 0, reent_context);
    std(((u8 *)reent_context + (0x23C)), 9, 1, reent_context);
    std(((u8 *)reent_context + (0x294)), 0xA, 2, reent_context);
    reent_context->__sglue_iobs = std_streams;
    reent_context->__sglue_niobs = 3;
    reent_context->__sglue_next = 0;
}
