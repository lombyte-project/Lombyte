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

/* exit x1: newlib exit.c with the game reent and JumpToRfuStatus as _exit. */

#include "types.h"
#include "rnc/sdk/library/stdio.h"

struct _atexit {
    struct _atexit *_next;
    int             _ind;
    void (*_fns[32])(void);
};

extern void JumpToRfuStatus(int code);

void exit(int code) {
    struct _atexit *atexit_block;
    int             callback_index;

    for (atexit_block = _impure_ptr->_atexit; atexit_block != 0; atexit_block = atexit_block->_next)
        for (callback_index = atexit_block->_ind; --callback_index >= 0;)
            (*atexit_block->_fns[callback_index])();

    if (_impure_ptr->__cleanup != 0)
        (*_impure_ptr->__cleanup)(_impure_ptr);

    JumpToRfuStatus(code);
}
