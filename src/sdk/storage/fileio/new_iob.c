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

/* new_iob n2: game FS I/O buffer allocator; get_iob sibling. */

#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sif_file_slot.h"

extern s32 _sceFsIobSemaMK();
extern s32 WaitSema();
extern s32 SignalSema();

void *new_iob(void) {
    struct SifFileSlot *iob;

    _sceFsIobSemaMK();
    WaitSema(fio_iob_sema);

    for (iob = fio_iob; iob < fio_iob + SIF_FILE_SLOT_COUNT; iob++) {
        if (iob->flags == 0) {
            iob->flags = SIF_FILE_SLOT_TAKEN;
            SignalSema(fio_iob_sema);
            return iob;
        }
    }

    SignalSema(fio_iob_sema);
    return 0;
}
