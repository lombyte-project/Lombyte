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

struct PrintIoVector {
    u8 reserved_00[4];
    s32 remaining_bytes;
    s32 vector_count;
};

extern s32 __sfvwrite();

s32 __sprint(s32 reent, struct PrintIoVector *vectors) {
    s32 result;

    if (vectors->vector_count == 0) {
        vectors->remaining_bytes = 0;
        return 0;
    }
    result = __sfvwrite(reent, vectors);
    vectors->vector_count = 0;
    vectors->remaining_bytes = 0;
    return result;
}
