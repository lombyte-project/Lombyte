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

extern s32 _sceFsIobSemaMK();
extern s32 WaitSema();
extern s32 SignalSema();
extern u8 D_00157D80[];

void *get_iob(u32 arg0) {
    void *result;

    _sceFsIobSemaMK();
    WaitSema(D_0012FCA0[0]);
    if (arg0 >= 32) {
        SignalSema(D_0012FCA0[0]);
        return 0;
    }
    result = (void *)&D_00157D80[arg0 << 4];
    SignalSema(D_0012FCA0[0]);
    return result;
}
