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
struct Reent {
    u8 pad_0[0x38];
    s32 unk38;
    s32 unk3C;
    u8 pad_40[0x198];
    s32 unk1D8;
    s32 unk1DC;
    s32 unk1E0;
};

extern u8 D_00113AC8[];
extern void std();
void __sinit(struct Reent *arg0) {
    s32 *temp_17_9;

    temp_17_9 = ((u8 *)arg0 + (0x1E4));
    arg0->unk3C = D_00113AC8;
    arg0->unk38 = 1;
    std(temp_17_9, 4, 0, arg0);
    std(((u8 *)arg0 + (0x23C)), 9, 1, arg0);
    std(((u8 *)arg0 + (0x294)), 0xA, 2, arg0);
    arg0->unk1E0 = temp_17_9;
    arg0->unk1DC = 3;
    arg0->unk1D8 = 0;
}
