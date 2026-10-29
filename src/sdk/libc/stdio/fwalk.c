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

struct ReentStream {
    u8 reserved_00[0xC];
    s16 flags;
};

struct ReentStreamBlock {
    struct ReentStreamBlock *next;
    s32 count;
    struct ReentStream *streams;
};

s32 _fwalk(u8 *reent, s32 (*callback)(void *)) {
    s32 stream_index;
    s32 result;
    struct ReentStream *stream;
    struct ReentStreamBlock *block;

    result = 0;
    block = (struct ReentStreamBlock *)(reent + 0x1D8);
    if (block != NULL) {
        do {
            stream_index = block->count;
            stream_index -= 1;
            stream = block->streams;
            if (stream_index >= 0) {
                do {
                    if (stream->flags != 0) {
                        result |= callback(stream);
                    }
                    stream_index -= 1;
                    stream = (struct ReentStream *)((u8 *)stream + 0x58);
                } while (stream_index >= 0);
            }
            block = block->next;
        } while (block != NULL);
    }
    return result;
}
